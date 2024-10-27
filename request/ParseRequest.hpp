#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP

#include "../server/Server.hpp"
#include "HTTPRequest.hpp"

class ParseRequest
{
	private:
		HTTPRequest _request;
	public:
		ParseRequest();
		~ParseRequest();
		void parse(char *buffer);
};

#endif

