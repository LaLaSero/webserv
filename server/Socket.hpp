/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:29:17 by ryanagit          #+#    #+#             */
/*   Updated: 2024/10/27 16:11:35 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <unistd.h> // closeのため
#include "SocketAddress.hpp" // 必要に応じてインクルード
#include "../config/Config.hpp"       // 必要に応じてインクルード

class Socket {
protected:
    int fd_;
    const SocketAddress server_addr_;
    const Config &config_;

public:
    Socket(int fd, const SocketAddress &server_addr, const Config &config);
    Socket(const Socket &rhs);
    virtual ~Socket() = 0; // 純粋仮想関数

    int GetFd() const;
    const Config &GetConfig() const;

private:
    Socket(); // デフォルトコンストラクタをプライベートに
    Socket &operator=(const Socket &rhs); // コピー代入演算子をプライベートに
};
