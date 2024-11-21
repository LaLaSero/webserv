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


void ClientSocket::SetResponse(const std::string& resp)
{
    response_ = resp;
}

const std::string ClientSocket::GetResponse(void) const
{
  return (response_);
}

ClientSocket::ClientSocket(int client_fd, SocketAddress client_address, const Config &conf): Socket(client_fd, client_address, conf) 
{
    std::cout << "ClientSocket created with fd: " << client_fd << std::endl;
}