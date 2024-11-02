/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:40:18 by ryanagit          #+#    #+#             */
/*   Updated: 2024/11/02 20:35:35 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include"Socket.hpp"
#include"ClientSocket.hpp"

class ListenSocket : public Socket {
 public:
  ListenSocket(int fd, const SocketAddress &server_addr, const Config &config);
//   // 現在の Socket に来た接続要求を accept する｡
//   // 返り値の Socket* はヒープ領域に存在しており､
//   // 解放するのは呼び出し側の責任である｡
  ClientSocket *AcceptNewConnection();

 private:
  // ListenSocket &operator=(const ListenSocket &rhs);
};

