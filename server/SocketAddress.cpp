/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketAddress.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 10:09:36 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/27 11:14:12 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"SocketAddress.hpp"
#include<cstring>   // for memcpy
#include<stdexcept> // for std::runtime_error

// デフォルトコンストラクタ
SocketAddress::SocketAddress() : socad_(NULL), length_(0) {}

// パラメータ付きコンストラクタ
SocketAddress::SocketAddress(const struct sockaddr *sockaddr, const socklen_t sockaddr_len)
{
    if (sockaddr == NULL || sockaddr_len == 0) {
        socad_ = NULL;
        length_ = 0;
    } else {
        socad_ = new struct sockaddr[sockaddr_len];
        std::memcpy(socad_, sockaddr, sockaddr_len);
        length_ = sockaddr_len;
    }
}

// コピーコンストラクタ
SocketAddress::SocketAddress(const SocketAddress &rhs)
{
    if (rhs.socad_ == NULL || rhs.length_ == 0) {
        socad_ = NULL;
        length_ = 0;
    } else {
        socad_ = new struct sockaddr[rhs.length_];
        std::memcpy(socad_, rhs.socad_, rhs.length_);
        length_ = rhs.length_;
    }
}

// 代入演算子
SocketAddress &SocketAddress::operator=(const SocketAddress &rhs)
{
    if (this != &rhs) {
        // 古いデータを解放
        if (socad_ != NULL) {
            delete[] socad_;
        }

        if (rhs.socad_ == NULL || rhs.length_ == 0) {
            socad_ = NULL;
            length_ = 0;
        } else {
            socad_ = new struct sockaddr[rhs.length_];
            std::memcpy(socad_, rhs.socad_, rhs.length_);
            length_ = rhs.length_;
        }
    }
    return *this;
}

// デストラクタ
SocketAddress::~SocketAddress()
{
    if (socad_ != NULL) {
        delete[] socad_;
    }
}

// ソケットアドレスを設定する
void SocketAddress::set_socad(struct sockaddr *soc)
{
    if (socad_ != NULL) {
        delete[] socad_;
    }
    if (soc != NULL) {
        length_ = sizeof(*soc);
        socad_ = new struct sockaddr[length_];
        std::memcpy(socad_, soc, length_);
    } else {
        socad_ = NULL;
        length_ = 0;
    }
}

// ソケットアドレスの長さを設定する
void SocketAddress::set_length(size_t length)
{
    if (length == 0) {
        throw std::runtime_error("Invalid length");
    }
    length_ = length;
}

// ソケットアドレスを取得する
struct sockaddr *SocketAddress::get_socad() const
{
    return socad_;
}

// ソケットアドレスの長さを取得する
size_t SocketAddress::get_length() const
{
    return length_;
}
