/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 13:38:42 by ryanagit          #+#    #+#             */
/*   Updated: 2024/12/01 20:18:53 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include<iostream>
#include"../config/Config.hpp"
#include"../config/Parser.hpp"
#include"EpollAdm.hpp"
#include"SocketAddress.hpp"
#include"ListenSocket.hpp"
#include"ClientSocket.hpp"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <signal.h>
// Socket関連
int PreparePassiveSocket(const char *host, const char *service, int type,
                      SocketAddress *sockaddr, bool doListen,
                      int backlog);

int InetListen(const std::string &host, const std::string &service, int backlog, SocketAddress *sockaddr);

// FdEvent関連
FdEvent *CreateFdEvent(int fd, FdFunc func, void *data);

// イベントハンドラ
void HandleClientSocketEvent(FdEvent *fde, unsigned int events, void *data, EpollAdm *epoll);
void HandleListenSocketEvent(FdEvent *fde, unsigned int events, void *data, EpollAdm *epoll);

// サーバーのセットアップ
void set_up_server(EpollAdm &epoll, Config &conf);

// イベントの実行
void AwakeFdEvent(FdEvent *fde, unsigned int events, EpollAdm *epoll);

// ループ処理
void Loop(EpollAdm &epoll);




#define BUF_SIZE 1024

