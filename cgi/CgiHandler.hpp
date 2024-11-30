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
#include "CgiResponse.hpp"

class CgiHandler
{
private:
	std::map<std::string, std::string> env_vars_;
	std::string request_body_;
	HTTPRequest &request_;

public:
	CgiHandler(HTTPRequest &request);
	void setRequestBody(const std::string &body);
	bool is_valid_root_and_executer_cgi();
	bool ExecuteCGI(std::string &response);

};

#endif
