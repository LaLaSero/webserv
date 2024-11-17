#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include "../request/HTTPRequest.hpp"

class CgiHandler
{
private:
	std::map<std::string, std::string> env_vars_;
	std::string request_body_;
	HTTPRequest &request_;

public:
	CgiHandler(HTTPRequest &request);
	void setRequestBody(const std::string &body);
	std::string ExecuteCGI();
	std::string ParseCGIresponse(const std::string &rawResponse);
};

#endif
