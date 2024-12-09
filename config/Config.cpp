/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:20:04 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/12/09 15:36:03 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Config.hpp"

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

void Config::AddFdandServers(int fd, ChildServer &Server)
{
    FdandServers_[fd] = Server;
}

const ChildServer& Config::FindServerfromFd(int fd, const std::string &hostname) const 
{
    // fd に対応するサーバーがマップに存在するか確認
    std::map<int, ChildServer>::const_iterator it = FdandServers_.find(fd);
    if (it == FdandServers_.end())
        throw std::runtime_error("Server not found for the given file descriptor.");
    const ChildServer &defaultServer = it->second;

    // 外側のループ
    for (std::vector<ChildServer>::const_iterator outerIt = ChildServers_.begin(); outerIt != ChildServers_.end(); ++outerIt) {
        if (outerIt->server_names_empty()) {
            return defaultServer;
        }
        const std::set<std::string>& serverNames = outerIt->get_server_names();

        // 内側のループ
    for (std::set<std::string>::const_iterator nameIt = serverNames.begin(); nameIt != serverNames.end(); ++nameIt) {
        if (*nameIt == hostname) {
            return *outerIt;
        }
    }
    }
    // 見つからない場合はデフォルトサーバーを返す
    return defaultServer;
}

void Config::displayConfig() const {
    //for debug
    // for (const auto& server : ChildServers_) {
    //     std::cout << "Server:\n";
    //     std::cout << "  Listen IP: " << server.get_listen_ip_() << "\n";
    //     std::cout << "  Listen Port: " << server.get_listen_port_() << "\n";

    //     if (server.server_names_empty()) {
    //         std::cout << "  Server Names: None\n";
    //     } else {
    //         std::cout << "  Server Names: ";
    //         for (const auto& name : server.get_server_names()) {
    //             std::cout << name << " ";
    //         }
    //         std::cout << "\n";
    //     }

    //     std::cout << "  Error Pages:\n";
    //     for (const auto& error : server.get_error_page()) {
    //         std::cout << "    " << error.first << ": " << error.second << "\n";
    //     }

    //     std::cout << "  Request Body Size: " << server.get_request_max() << "\n";

    //     std::cout << "  Locations:\n";
    //     for (const auto& location : server.getLocations()) {
    //         std::cout << "    Path: " << location.getPath() << "\n";
    //         std::cout << "    Accepted Methods: ";
    //         for (const auto& method : location.getAcceptedMethods()) {
    //             std::cout << method << " ";
    //         }
    //         std::cout << "\n";
    //         std::cout << "    Root Directory: " << location.getRootDirectory() << "\n";
    //         std::cout << "    Directory Listing: " << (location.isDirectoryListing() ? "Enabled" : "Disabled") << "\n";
    //         std::cout << "    Default File: " << location.getDefaultFile() << "\n";
    //         std::cout << "    Upload Directory: " << location.getUploadDirectory() << "\n";
    //         std::cout << "    Client Max Body Size: " << location.getClientMaxBodySize() << "\n";
    //         std::cout << "    CGI Extensions:\n";
    //         for (const auto& cgi : location.getCgiExtension()) {
    //             std::cout << "      " << cgi.first << " -> " << cgi.second << "\n";
    //         }
    //         std::cout << "\n";
    //     }
    // }
}


