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
      void SetResponse(const std::string& resp);
      const std::string GetResponse(void) const;
      ClientSocket(int client_fd, SocketAddress client_address, const Config &conf);
 private:
  std::string response_;
  ClientSocket();
  ClientSocket &operator=(const ClientSocket &rhs);
};