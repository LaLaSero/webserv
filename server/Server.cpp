#include "Server.hpp"

Server::Server()
{
	_server_map = {
		{80, {
			ServerConfig("example.com", "/var/www/example", 1048576),
			ServerConfig("blog.example.com", "/var/www/blog", 1572864)
		}},
		{8080, {
			ServerConfig("test.com", "/var/www/test", 2097152)
		}}
	};
}

Server::~Server()
{

}



void
Server::start()
{
	std::cout << "Server started" << std::endl;
	set_server();
	std::map<int, std::vector<ServerConfig>>::const_iterator it = _server_map.cbegin();
	for (; it != _server_map.cend(); ++it)
	{
		std::cout << "Server listening on port " << it->first << std::endl;
	}
	loop();
}