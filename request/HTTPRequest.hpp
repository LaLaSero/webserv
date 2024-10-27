#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "../server/Server.hpp"

class HTTPRequest
{
	private:
		Server _server;
		std::string _method;
		std::string _uri;
		std::string _version;
		std::string _hostname;
		std::string _path;
		std::string _query;
		std::string _bodysize;


	public:
		HTTPRequest();
		~HTTPRequest();
};

#endif