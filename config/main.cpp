/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanagitaryusei <yanagitaryusei@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:07:54 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/19 19:07:00 by yanagitaryu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
#include"Config.hpp"
#include"Parser.hpp"

#define DEFAULT_PATH "./Makefile"

Config parse(std::string path);

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
	try
	{
		Parser pas;
		pas.LoadFile(config_file_path);
		Config conf = pas.MakeConfig();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	
}