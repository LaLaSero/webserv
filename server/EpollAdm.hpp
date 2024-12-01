/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollAdm.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:17:30 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/27 10:11:36 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include"../config/Config.hpp"
#include <sys/epoll.h>
#include<iostream>
#include <cassert>
#include <ctime>
#include <map>
#include <vector>
#include <sys/time.h>
#include"ClientSocket.hpp"

enum EFdeEvent {
  kFdeRead = 0x0001,
  kFdeWrite = 0x0002,
  kFdeError = 0x0004,
  kFdeTimeout = 0x0008
};

struct FdEvent;
class EpollAdm;

typedef void (*FdFunc)(FdEvent *fde, unsigned int events, void *data,
                       EpollAdm *epoll);

struct FdEvent {
  int fd;
  FdFunc func;

  long timeout_ms;
  long last_active;


  // 監視対象のFdeEvent
  unsigned int state;

  void *data;

 ClientSocket *original_clinet;
};

struct FdandEvent {
  FdEvent *fde;

  unsigned int events;
};



class EpollAdm
{
	public:
    EpollAdm(const Config &conf);
    const Config& get_config()const;
    void register_event(FdEvent *fde);
    void delete_event(FdEvent *fde);
    void Set(FdEvent *fde, unsigned int events);
    void Add(FdEvent *fde, unsigned int events);
    std::vector<FdandEvent> RetrieveTimeouts();
    std::vector<FdandEvent> CheckEvents(int timeout_ms);
    void GotoNextEvent(FdEvent *fde, unsigned int events);
	private:
    const int epfd_;
    std::map<int, FdEvent *> registered_fd_events_;
    const Config& config_;
};