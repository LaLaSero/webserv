/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanagitaryusei <yanagitaryusei@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:14:53 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/24 20:38:45 by yanagitaryu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Server.hpp"

std::string Server::get_listen_ip_() const 
{
    return listen_ip_;
}

std::string Server::get_listen_port_() const 
{
    return listen_port_;
}

void Server::set_listen_ip_(const std::string& listen_ip) {
    listen_ip_ = listen_ip;
}

void Server::set_listen_port_(const std::string& listen_port) {
    listen_port_ = listen_port;
}

void Server::add_server_names_(const std::string& server_name)
{
	server_names_.insert(server_name);
}

void Server::add_error_page_(const int i, const std::string& page)
{
	error_page_.insert(std::pair<int, std::string>(i, page));
}

bool Server::server_names_empty()const
{
	if (server_names_.empty())
		return (true);
	else
		return (false);
}

void Server::set_request_max(const size_t i)
{
	reqbody_size_ = i;
}