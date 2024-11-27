/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChildServer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanagitaryusei <yanagitaryusei@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:14:53 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/26 16:14:32 by yanagitaryu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ChildServer.hpp"

ChildServer::ChildServer():reqbody_size_(req_max_body_size)
{
}


std::string ChildServer::get_listen_ip_() const 
{
    return listen_ip_;
}

std::string ChildServer::get_listen_port_() const 
{
    return listen_port_;
}

// Getter メソッドの実装
const std::set<std::string>& ChildServer::get_server_names() const {
    return server_names_;
}

const std::map<int, std::string>& ChildServer::get_error_page() const {
    return error_page_;
}

size_t ChildServer::get_request_max() const {
    return reqbody_size_;
}


const std::vector<Location>& ChildServer::getLocations() const {
    return locations_;
}

void ChildServer::set_listen_ip_(const std::string& listen_ip) {
    listen_ip_ = listen_ip;
}

void ChildServer::set_listen_port_(const std::string& listen_port) {
    listen_port_ = listen_port;
}

void ChildServer::add_server_names_(const std::string& server_name)
{
	server_names_.insert(server_name);
}

void ChildServer::add_error_page_(const int i, const std::string& page)
{
	error_page_.insert(std::pair<int, std::string>(i, page));
}

bool ChildServer::server_names_empty()const
{
	if (server_names_.empty())
		return (true);
	else
		return (false);
}

void ChildServer::set_request_max(const size_t i)
{
	reqbody_size_ = i;
}

void ChildServer::add_location(const Location &loc)
{
	locations_.push_back(loc);
}