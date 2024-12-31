/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollAdm.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:17:47 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/27 10:11:38 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "EpollAdm.hpp"
#include "ServerException.hpp"

long GetNowTime() {
  timeval tv;

  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

epoll_event MakeEpollEvent(FdEvent *fde) 
{
  epoll_event epev;
  epev.events = 0;
  if (fde->state & kFdeRead) {
    epev.events |= EPOLLIN;
  }
  if (fde->state & kFdeWrite) {
    epev.events |= EPOLLOUT;
  }
  epev.events |= EPOLLRDHUP;
  epev.data.fd = fde->fd;
  return epev;
}


FdandEvent MakeFdandEvent(FdEvent *fde, epoll_event epev) 
{
  unsigned int events = 0;
  if ((epev.events & EPOLLIN) && (fde->state & kFdeRead)) 
    events |= kFdeRead;
  if ((epev.events & EPOLLOUT) && (fde->state & kFdeWrite)) 
    events |= kFdeWrite;
  if (epev.events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) 
  {
    if (epev.events & EPOLLERR)
    {
      throw ServerException(HTTP_INTERNAL_SERVER_ERROR, "Epoll error occurred");
    }
    if (epev.events & EPOLLHUP)
    {
      throw ServerException(HTTP_REQUEST_TIMEOUT, "Epoll hang up occurred");
    }
    if (epev.events & EPOLLRDHUP) 
    {
      throw ServerException(HTTP_REQUEST_TIMEOUT, "Epoll read hang up occurred");
    }
    events |= kFdeRead | kFdeError;
  }
  FdandEvent fdee;
  fdee.fde = fde;
  fdee.events = events;
  fde->last_active = GetNowTime();
  return fdee;
}

EpollAdm::EpollAdm(const Config &conf):epfd_(epoll_create1(0)),config_(conf)
{
    if (epfd_ == -1) 
      throw std::runtime_error("Failed to create epoll file descriptor");
}

void EpollAdm::register_event(FdEvent *fde) 
{
  if(registered_fd_events_.find(fde->fd) != registered_fd_events_.end())
        throw std::runtime_error("register_event Error");
  epoll_event epev = MakeEpollEvent(fde);
  if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fde->fd, &epev) < 0) 
        throw std::runtime_error("Epoll Register Error");
  registered_fd_events_[fde->fd] = fde;
}

void EpollAdm::delete_event(FdEvent *fde)
{
    std::map<int,FdEvent*>::iterator it = registered_fd_events_.find(fde->fd);
    if (it == registered_fd_events_.end()) 
        throw std::runtime_error("delete_event Error: File descriptor not registered");
    // epoll_ctl を使ってイベントを削除
    // std::cout << epfd_ << std::endl;
    // std::cout << fde->fd << std::endl;
    if (epoll_ctl(epfd_, EPOLL_CTL_DEL, fde->fd, NULL) < 0) {
        throw std::runtime_error("Epoll Delete Error");
    }

    // 登録されたイベントリストからファイルディスクリプタを削除
    registered_fd_events_.erase(it);

    // FdEvent のメモリを解放
    delete fde;
}


void EpollAdm::Set(FdEvent *fde, unsigned int events) {
  unsigned int previous_state = fde->state;
  fde->state = events;
  // 前回と同じだったら epoll_ctl は実行しない
  // kFdeTimeout が変わっても epoll を変更する必要はない
  if ((fde->state % ~kFdeTimeout) == (previous_state % ~kFdeTimeout)) 
    return;
  epoll_event epev = MakeEpollEvent(fde);
  if (epoll_ctl(epfd_, EPOLL_CTL_MOD, fde->fd, &epev) < 0)
        throw std::runtime_error("Epoll Set Error:epoll_ctl failed");
}

void EpollAdm::Add(FdEvent *fde, unsigned int events) {
  Set(fde, fde->state | events);
}

std::vector<FdandEvent> EpollAdm::RetrieveTimeouts() 
{
  std::vector<FdandEvent> fdee_vec;

  // std::cout << registered_fd_events_.size() << std::endl;
  long current_time = GetNowTime();
  for (std::map<int, FdEvent *>::const_iterator it = registered_fd_events_.begin(); it != registered_fd_events_.end(); ++it) 
  {
    FdEvent *fde = it->second;
    if (fde->state / kFdeTimeout && current_time - fde->last_active > fde->timeout_ms) 
    {
		std::cerr << "\x1b[38;2;10;20;30mDebug" << std::endl;
		std::cerr << "Timeout occurred fd:" << fde->fd <<std::endl;
		std::cerr << "current_time:" << current_time << std::endl;
		std::cerr << "last_active:" << fde->last_active << std::endl;
		std::cerr << "timeout_ms:" << fde->timeout_ms << std::endl;
		std::cerr << "\x1b[0m" << std::endl;

      FdandEvent fdee;
      fdee.fde = fde;
      fdee.events = kFdeTimeout | kFdeRead;
      fdee_vec.push_back(fdee);
    }
  }
  return fdee_vec;
}

std::vector<FdandEvent> EpollAdm::CheckEvents(int timeout_ms) 
{
    std::vector<FdandEvent> fdee_vec;
    std::vector<epoll_event> epoll_events;

    epoll_events.resize(registered_fd_events_.size());
    int event_num = epoll_wait(epfd_, epoll_events.data(), epoll_events.size(), timeout_ms);
    if (event_num < 0) 
        throw std::runtime_error("Error occurred in epoll_wait");
    for (int i = 0; i < event_num; ++i) 
    {
        if (registered_fd_events_.find(epoll_events[i].data.fd) == registered_fd_events_.end())
        {
          //epoll wait detectes non_registerd_fd_events
            throw std::runtime_error("Error occurred in Wait event");
        }
        FdEvent *fde = registered_fd_events_[epoll_events[i].data.fd];
        // FdandEventの作成と追加
        FdandEvent fdee = MakeFdandEvent(fde,epoll_events[i]); // 実際のイベントを設定
        fdee_vec.push_back(fdee);
        fde->last_active = GetNowTime();
		std::cerr << "-----Debug-------" << std::endl;
		std::cerr << "CheckEvents occurred fd:" << fde->fd <<std::endl;
		std::cerr << "current_time:" << fde->last_active << std::endl;
		std::cerr << "update last_active" << std::endl;
    }
    return fdee_vec;
}


void EpollAdm::GotoNextEvent(FdEvent *fde, unsigned int events)
{
    // 現在の状態を取得
    unsigned int previous_state = fde->state;

    // 新しい状態を設定
    fde->state |= events; // 現在の状態に追加する形でイベントを設定

    // 前回の状態と同じ場合、何も変更する必要はない
    if ((fde->state % kFdeTimeout) == (previous_state % kFdeTimeout))
        return; // 変更がないためリターン
    epoll_event epev = MakeEpollEvent(fde);
    // epoll_ctl を使ってイベントを変更
    if (epoll_ctl(epfd_, EPOLL_CTL_MOD, fde->fd, &epev) < 0) 
        throw std::runtime_error("Epoll GotoNextEvent Error: epoll_ctl failed");
}


const Config& EpollAdm::get_config()const
{
  return (config_);
}