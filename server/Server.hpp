#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <exception>
#include <cstring>
#include <map>
#include <vector>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

#define AF_INET 2
#define SOCK_STREAM 1
#define INADDR_ANY 0

// let me suppose _server_map to be like this:
// _servers = {
//     {80, {
//         ServerConfig("example.com", "/var/www/example", 1048576),
//         ServerConfig("blog.example.com", "/var/www/blog", 1572864)
//     }},
//     {8080, {
//         ServerConfig("test.com", "/var/www/test", 2097152)
//     }}
// };
struct ServerConfig
{
	std::string server_name;
	std::string root;
	int max_body_size;

	ServerConfig(std::string server_name, std::string root, int max_body_size)
	{
		this->server_name = server_name;
		this->root = root;
		this->max_body_size = max_body_size;
	}
};

class Server
{
	private:
		struct	addrinfo _hints;
		struct	addrinfo *_res;
		char	_buffer[BUFFER_SIZE];
		int		_server_fd;
		std::map<int, std::vector<ServerConfig> > _server_map;
		void loop();


	public:
		Server();
		~Server();
		void set_server();
		void start();
	
};
