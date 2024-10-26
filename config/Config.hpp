/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanagitaryusei <yanagitaryusei@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:19:40 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/26 17:02:38 by yanagitaryu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include<iostream>
#include"ChildServer.hpp"

class Config
{
	public:
		Config();
		~Config();
		Config(const Config &other);
		Config &operator=(const Config &other);

		void AddChildServer(const ChildServer &server);
	private:
		std::vector<ChildServer> ChildServers_;
};