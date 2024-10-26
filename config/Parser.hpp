/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanagitaryusei <yanagitaryusei@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:04:00 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/25 20:12:44 by yanagitaryu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<stdexcept>  
#include"Config.hpp"
#include"Server.hpp"
#include<vector>

#define kAnyIpAddress "0.0.0.0"
#define PortMax 65535

class Parser
{
	private:
    	std::istringstream content_;
	public:
		Parser();
		void LoadFile(const std::string &file_path);
		Config MakeConfig();
		void MakeServer(Config &conf);

		void ValidListen(std::string &listen_info);
		void ValidIp(std::string &ip);
		void ValidPort(std::string &port);
		void ParseListen(Server &serv, std::string &line);

		void ValidServerName(const std::string& str);
		void ParseServname(Server &serv, std::string &line);

		void ValidErrorstatus(std::string &status);
		void ParseErrorpage(Server &serv, std::string &line);
		
		size_t max_stos(std::string &max);
		void ParseCLMAX(Server &serv, std::string &line);
		void ParseRewrite(Location &loc, std::string &line);

		void ParseLocation(Server &serv, std::string &line);
		
};