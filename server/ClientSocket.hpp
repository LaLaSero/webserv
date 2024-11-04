/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:40:47 by ryanagit          #+#    #+#             */
/*   Updated: 2024/11/02 11:43:19 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#include"Socket.hpp"
#include"SocketAddress.hpp"

class ClientSocket : public Socket {
 public:
      const std::string GetResponse(void) const;
      void SetResponse(std::string res);
      ClientSocket(int client_fd, SocketAddress client_address, const Config &conf);
      ClientSocket(int fd, const SocketAddress &server_addr,
                       const SocketAddress &client_addr,
                       const Config &config) ;
 private:
  const SocketAddress client_addr_;
  std::string response_;
  bool is_shutdown_;
  ClientSocket();
  ClientSocket &operator=(const ClientSocket &rhs);
};