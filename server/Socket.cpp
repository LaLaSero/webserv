/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:30:25 by ryanagit          #+#    #+#             */
/*   Updated: 2024/11/30 15:30:27 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Socket.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h> // for close()
#include <string>
#include <iostream>
#include <unistd.h> // for close()

// コンストラクタの実装
Socket::Socket(int fd, const SocketAddress &server_addr, const Config &config)
    : fd_(fd), server_addr_(server_addr), config_(config) {
    // 必要な初期化処理をここに追加
}

// コピーコンストラクタの実装
Socket::Socket(const Socket &rhs)
    : fd_(rhs.fd_), server_addr_(rhs.server_addr_), config_(rhs.config_) {
    // 必要に応じて追加の処理をここに記述
}

// デストラクタの実装
Socket::~Socket() {
    close(fd_); // ソケットを閉じる
}

// GetFd メソッドの実装
int Socket::GetFd() const {
    return fd_;
}

// GetConfig メソッドの実装
const Config &Socket::GetConfig() const {
    return config_;
}
