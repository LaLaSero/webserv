#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include "HTTPRequest.hpp"

class HTTPResponse
{
	public:
		HTTPResponse();
		~HTTPResponse();
		
		void clear();

	private:
		std::string _version;
		// HTTPStatusCode _statusCode;
		std::string _statusMessage;
		std::string _statusLine;
		bool _keepAlive;
		size_t _contentLength;
		std::map<std::string, std::string> _headers;
		std::string _body;
		std::string _statusMessage;


		bool _keepAlive;

};

#endif