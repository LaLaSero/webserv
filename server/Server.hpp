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
#include <cstdlib>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

#ifndef AF_INET
#define AF_INET 2
#endif

#ifndef SOCK_STREAM
#define SOCK_STREAM 1
#endif

#ifndef INADDR_ANY
#define INADDR_ANY 0
#endif

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
		: server_name(server_name), root(root), max_body_size(max_body_size)
	{}
};
class Server
{
	private:
		struct	addrinfo _hints;
		struct	addrinfo *_res;
		char	_buffer[BUFFER_SIZE];
		// int		_server_fd;
		std::map<int, std::vector<ServerConfig> > _server_map;
		std::map<std::string, int> _fd_map;

		void alocateServerAddr(std::string port);
		void createSocket(std::string port);
		void bindSocket(int fd);
		void listenSocket(int fd);
		int solve_fd(std::string port);
		void loop();
		void makePortFdMap(std::string port, int fd);
		void closeUsedFd();
	


	public:
		Server();
		~Server();
		void set_server();
		void start();
	
};
