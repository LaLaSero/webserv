#include "Server.hpp"

Server::Server()
{
	// ポート80用の設定
	std::vector<ServerConfig> configs80;
	configs80.push_back(ServerConfig("example.com", "/var/www/example", 1048576));
	configs80.push_back(ServerConfig("blog.example.com", "/var/www/blog", 1572864));
	_server_map[80] = configs80;

	// ポート8080用の設定
	std::vector<ServerConfig> configs8080;
	configs8080.push_back(ServerConfig("test.com", "/var/www/test", 2097152));
	_server_map[8080] = configs8080;
}

Server::~Server()
{

}

void
Server::set_server()
{
	// サーバーの設定を行う
}

void
Server::loop()
{

}

void
Server::start()
{
	std::cout << "Server started" << std::endl;
	set_server();
	std::map<int, std::vector<ServerConfig> >::const_iterator it = _server_map.begin();
	for (; it != _server_map.end(); ++it)
	{
		std::cout << "port " << it->first << std::endl;
	}
	loop();
}