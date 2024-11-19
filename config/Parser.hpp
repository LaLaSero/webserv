/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 15:04:00 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/27 10:11:45 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<stdexcept>  
#include"Config.hpp"
#include"ChildServer.hpp"
#include<vector>

#define kAnyIpAddress "0.0.0.0"
#define PortMax 65535

class Parser
{
	private:
    	std::istringstream content_;
	public:
		Parser();
		~Parser();
		Parser(const Parser& ohter);
		Parser& operator=(const Parser& other);
		void LoadFile(const std::string &file_path);
		Config MakeConfig();
		void MakeChildServer(Config &conf);

		void ValidListen(std::string &listen_info);
		void ValidIp(std::string &ip);
		void ValidPort(std::string &port);
		void ParseListen(ChildServer &serv, std::string &line);

		void ValidChildServerName(const std::string& str);
		void ParseServname(ChildServer &serv, std::string &line);

		void ValidErrorstatus(std::string &status);
		void ParseErrorpage(ChildServer &serv, std::string &line);
		
		size_t max_stos(std::string &max);
		void ParseCLMAX(ChildServer &serv, std::string &line);
		void ParseRewrite(Location &loc, std::string &line);

		void ParseLocation(ChildServer &serv, std::string &line);


		bool check_syntax(std::string &line, std::string target, bool flag);
		
};