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

class CgiHandler
{
private:
	std::string script_path_;
	std::map<std::string, std::string> env_vars_;
	std::string request_method_;
	std::string request_body_;

public:
	CgiHandler(const std::string &scriptPath, const std::map<std::string, std::string> &envVars);
	void setScriptPath(const std::string &scriptPath);
	void setRequestMethod(const std::string &method);
	void setRequestBody(const std::string &body);
	std::string ExecuteCGI();
	std::string ParseCGIresponse(const std::string &rawResponse);
};

#endif
