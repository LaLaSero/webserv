/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:07:54 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/11/22 18:29:44 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
#include"config/Config.hpp"
#include"config/Parser.hpp"
#include"server/EpollAdm.hpp"
#include"server/SocketAddress.hpp"
#include"server/ListenSocket.hpp"
#include"server/ClientSocket.hpp"
#include"server/functions.hpp"

#define DEFAULT_PATH "test.conf"


int main(int argc, char *argv[])
{
	std::string config_file_path = DEFAULT_PATH;
	if (2 < argc)
	{
		std::cerr << "Error:argv" << std::endl;
		return (1);
	}
	if (argc == 2)
		config_file_path = argv[1];
	Parser pas;
	Config conf;
	EpollAdm epo(conf);
	try
	{
		pas.LoadFile(config_file_path);
		conf = pas.MakeConfig();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::exit(1);
	}

	try
	{
		set_up_server(epo, conf);
		// conf.displayConfig();
    	Loop(epo);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::exit(1);
	}
	
}