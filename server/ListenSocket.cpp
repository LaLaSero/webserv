/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:02:19 by ryanagit          #+#    #+#             */
/*   Updated: 2024/11/02 15:22:13 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ListenSocket.hpp"

// ListenSocket クラスのコンストラクタ


ListenSocket::ListenSocket(int fd, const SocketAddress &server_addr, const Config &config)
    : Socket(fd, server_addr, config) {}