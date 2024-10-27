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

#include"EpollAdm.hpp"

epoll_event CalculateEpollEvent(FdEvent *fde) 
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


EpollAdm::EpollAdm():epfd_(epoll_create1(0)) 
{
    if (epfd_ == -1) 
    {
            throw std::runtime_error("Failed to create epoll file descriptor");
    }
}

void EpollAdm::register_event(FdEvent *fde) 
{
  if(registered_fd_events_.find(fde->fd) != registered_fd_events_.end())
        throw std::runtime_error("register_event Error");
  epoll_event epev = CalculateEpollEvent(fde);

  if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fde->fd, &epev) < 0) {
        throw std::runtime_error("Epoll Register Error");
  }
  registered_fd_events_[fde->fd] = fde;
}

void EpollAdm::Set(FdEvent *fde, unsigned int events) {
  unsigned int previous_state = fde->state;
  fde->state = events;
  // 前回と同じだったら epoll_ctl は実行しない
  // kFdeTimeout が変わっても epoll を変更する必要はない
  if ((fde->state & ~kFdeTimeout) == (previous_state & ~kFdeTimeout)) {
    return;
  }

  epoll_event epev = CalculateEpollEvent(fde);
  if (epoll_ctl(epfd_, EPOLL_CTL_MOD, fde->fd, &epev) < 0) {
        throw std::runtime_error("Epoll Set Error");
  }
}

void EpollAdm::Add(FdEvent *fde, unsigned int events) {
  Set(fde, fde->state | events);
}