/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChildServer.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanagitaryusei <yanagitaryusei@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:14:58 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/26 16:13:44 by yanagitaryu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<stdexcept>
#include<set>
#include<map>
#include<vector>
#include"Location.hpp"


#define DEFAULT_RBSIZE 1048576

class ChildServer
{
	private:
		std::string listen_ip_;
		std::string listen_port_;
		std::set<std::string> server_names_;
		std::map<int, std::string> error_page_;
		size_t		reqbody_size_;
		std::vector<Location>	locations_;
	public:
		ChildServer();
		std::string get_listen_ip_() const;
		std::string get_listen_port_() const;
    	const std::set<std::string>& get_server_names() const; // 新しい getter
    	const std::map<int, std::string>& get_error_page() const; // 新しい getter
    	size_t get_request_max() const; // 新しい getter
		const std::vector<Location>& getLocations() const;
    	void set_listen_ip_(const std::string& listen_ip);
    	void set_listen_port_(const std::string& listen_port);
		void add_server_names_(const std::string& server_name);
		void add_error_page_(const int i, const std::string& page);
		void set_request_max(const size_t i);
		bool server_names_empty() const;
		void add_location(const Location &loc);
};