/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketAddress.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 10:09:32 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/27 10:32:26 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>

class SocketAddress
{
	public:
		SocketAddress();
		SocketAddress(const struct sockaddr *sockaddr, const socklen_t sockaddr_len);
		SocketAddress(const SocketAddress &rhs);
		SocketAddress &operator=(const SocketAddress &rhs);
		~SocketAddress();
		void set_socad(struct sockaddr *soc);
		void set_length(size_t length);
		struct sockaddr *get_socad()const;
		size_t get_length()const;
	private:
		struct sockaddr *socad_;
		size_t length_;
};