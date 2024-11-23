/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:19:40 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/11/23 15:19:05 by ryanagit         ###   ########.fr       */
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
    	void displayConfig() const;
		void AddFdandServers(int fd, ChildServer &Server);
		const ChildServer &FindServerfromFd(int fd)const;
	private:
		std::vector<ChildServer> ChildServers_;
		std::map<int, ChildServer> FdandServers_;
};