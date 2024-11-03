/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 15:02:19 by ryanagit          #+#    #+#             */
/*   Updated: 2024/11/03 12:07:25 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ListenSocket.hpp"

// ListenSocket クラスのコンストラクタ


ListenSocket::ListenSocket(int fd, const SocketAddress &server_addr, const Config &config)
    : Socket(fd, server_addr, config) {}


ClientSocket *ListenSocket::AcceptNewConnection() 
{
  struct sockaddr_storage client_addr;
  socklen_t addrlen = sizeof(struct sockaddr_storage);
  // std::cout << fd_  << std::endl;
  int conn_fd = accept4(fd_, (struct sockaddr *)&client_addr, &addrlen,
                        SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (conn_fd < 0) 
  {
    perror("accept4");
    throw std::exception();
  }
  ClientSocket *conn_sock = new ClientSocket(
      conn_fd, server_addr_,
      SocketAddress((const struct sockaddr *)&client_addr, addrlen), config_);
  return conn_sock;
}
