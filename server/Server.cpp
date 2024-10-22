#include "Server.hpp"

Server::Server()
{
	memset(&_hints, 0, sizeof(addrinfo));
	_res = NULL;
	memset(_buffer, 0, sizeof(char) * BUFFER_SIZE);
}

Server::~Server()
{
}

// 擬似的にサーバーの設定を行うが，最終的にはConfigクラスからの読み込みを行う．
void
Server::set_server()
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

void
Server::alocateServerAddr(std::string port)
{
	// サーバーアドレスを確保する
	char *port_str = const_cast<char *>(port.c_str());
	memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, port_str, &_hints, &_res) != 0 )
	{
		std::cerr << "fuck getaddrinfo" << std::endl;
		std::exit(1);
	}
}


void
Server::makePortFdMap(std::string port, int fd)
{
	_fd_map[port] = fd;
}

void
Server::createSocket(std::string port)
{
	int fd;

	// socketを作る
	fd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	if (fd < 0)
	{
		std::cout << "fuck socket" << std::endl;
		std::exit(1);
	}
	// non-blockingで動作させる
	makePortFdMap(port, fd);
}

void
Server::bindSocket(int fd)
{
	// ソケットにアドレスをバインドする
	(void)fd;
}

void
Server::listenSocket(int fd)
{
	// ソケットをリッスンする
	(void) fd;
}

void
Server::loop()
{
	// サーバーのメインループ
}

int
Server::solve_fd(std::string port)
{
	return (_fd_map[port]);
}

void
Server::closeUsedFd()
{
	std::map<std::string, int>::iterator it = _fd_map.begin();
	for(; it != _fd_map.end(); ++it)
	{
		close(it->second);
	}
}

void
Server::start()
{
	std::cout << "Server started" << std::endl;
	set_server();
	std::map<int, std::vector<ServerConfig> >::const_iterator it = _server_map.begin();
	for (; it != _server_map.end(); ++it)
	{
		std::string current_port = std::to_string(it->first);
		std::cout << "current_port " << current_port << std::endl;
		std::vector<ServerConfig> current_config = it->second;
		std::vector<ServerConfig>::const_iterator it2 = it->second.begin();
		for (; it2 != it->second.end(); ++it2)
		{
			std::cout << "  server_name: " << it2->server_name << std::endl;
			std::cout << "  root: " << it2->root << std::endl;
			std::cout << "  max_body_size: " << it2->max_body_size << std::endl;
		}
		alocateServerAddr(current_port);
		createSocket(current_port);
		int current_fd = solve_fd(current_port);
		// bindSocket(current_fd);
		// listenSocket(current_fd);
		std::cout << "current_fd: " << current_fd << std::endl;
	}
	loop();
	closeUsedFd();
}