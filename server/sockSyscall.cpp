#include "Server.hpp"

void
Server::alocateServerAddr(std::string port)
{
	const char *port_str = port.c_str();
	memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	int status = getaddrinfo(NULL, port_str, &_hints, &_res);
	if (status != 0)
		sockError("getaddrinfo: ", -1, status);
}

void
Server::createSocket(std::string port)
{
	int fd;

	fd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	if (fd < 0)
		sockError("socket: ", -1, errno);
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
		sockError("non blocking", fd, errno);
	makePortFdMap(port, fd);
}

void
Server::bindSocket(int fd)
{
	if (bind(fd, _res->ai_addr, _res->ai_addrlen) < 0)
	{
		closeUsedFd();
		sockError("bind: ", -1, errno);
	}
	freeaddrinfo(_res);
	_res = NULL;
}

void
Server::listenSocket(int fd)
{
	if (listen(fd, QUEUE_SIZE) < 0)
		sockError("listen :", fd, errno);
}