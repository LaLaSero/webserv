#ifndef CGIRESPONSE_HPP
#define CGIRESPONSE_HPP

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include "../request/HTTPRequest.hpp"

std::string ParseCGIResponse(const std::string &response, bool &local_redirect_flag);

#endif