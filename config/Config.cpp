/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:20:04 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/27 10:11:57 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Config.hpp"
#include "Config.hpp"

// デフォルトコンストラクタ
Config::Config()
{
    // 必要なら初期化処理をここに書く
}

// デストラクタ
Config::~Config()
{
    // 自動的にvectorのデストラクタが呼ばれるため、特別な処理は不要
}

// コピーコンストラクタ
Config::Config(const Config& other)
{
    ChildServers_ = other.ChildServers_;
}

// コピー代入演算子
Config& Config::operator=(const Config& other)
{
    if (this != &other)
    {
        ChildServers_ = other.ChildServers_;
    }
    return *this;
}


void Config::addChildServer(const ChildServer &server)
{
	ChildServers_.push_back(server);
}

std::vector<ChildServer> Config::getchildserver()const
{
	return(ChildServers_);
}
