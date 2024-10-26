/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yanagitaryusei <yanagitaryusei@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:20:04 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/26 17:02:38 by yanagitaryu      ###   ########.fr       */
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


void Config::AddChildServer(const ChildServer &server)
{
	ChildServers_.push_back(server);
}
