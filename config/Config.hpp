/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:19:40 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/27 15:58:37 by ryanagit         ###   ########.fr       */
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

		void addChildServer(const ChildServer &server);
		std::vector<ChildServer> getchildserver()const;
	private:
		std::vector<ChildServer> ChildServers_;
};