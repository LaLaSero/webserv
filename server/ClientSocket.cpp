/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:40:49 by ryanagit          #+#    #+#             */
/*   Updated: 2024/10/27 14:42:29 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ClientSocket.hpp"

const std::string ClientSocket::GetResponse(void) const
{
  return (response_);
}

void ClientSocket::SetResponse(std::string res)
{
  response_ = res;
}

ClientSocket::ClientSocket(int client_fd, SocketAddress client_address, const Config &conf): Socket(client_fd, client_address, conf) 
{
}
ClientSocket::ClientSocket(int fd, const SocketAddress &server_addr,
                       const SocketAddress &client_addr,
                       const Config &config, int server_fd) : Socket(fd, server_addr, config),
      client_addr_(client_addr),
      is_shutdown_(false),
      server_fd_(server_fd) {}


int ClientSocket::get_server_fd()const
{
  return (server_fd_);
}