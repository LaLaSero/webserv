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

ClientSocket::ClientSocket(int client_fd, SocketAddress client_address, const Config &conf): Socket(client_fd, client_address, conf) 
{
}
ClientSocket::ClientSocket(int fd, const SocketAddress &server_addr,
                       const SocketAddress &client_addr,
                       const Config &config) : Socket(fd, server_addr, config),
      client_addr_(client_addr),
      response_(NULL),
      is_shutdown_(false) {}
