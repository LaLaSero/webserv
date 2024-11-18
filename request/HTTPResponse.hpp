#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include "HTTPRequest.hpp"
#include "HTTPStatusCode.hpp"
#include <ctime>
#include <cstdio>

#ifndef SERVER_NAME
#define SERVER_NAME "webserv"
#endif

#define NORMAL_RESPONSE 0
#define REDIRECT_RESPONSE 1
#define CGI_RESPONSE 2


class HTTPResponse
{
	public:
		HTTPResponse();
		~HTTPResponse();
		
		void clear();
		void setStatusMessage(const std::string& statusMessage);

		std::string getCurrentTime();
		void setHeader(const std::string& key, const std::string& value);

		void setStatusline();

		void makeMessage();

		void selectResponseMode(HTTPRequest& request);
		void setStatusMessageMap(void);
	
		void handleNormalRequest(HTTPRequest& request);

		void makeBodyGET(HTTPRequest& request);
		void makeBodyPOST(HTTPRequest& request);
		void makeBodyDELETE(HTTPRequest& request);

		bool isGCIRequest(HTTPRequest& request);
		void handleCGIRequest(HTTPRequest& request);

		bool isRedirectRequest(HTTPRequest& request);
		void handleRedirectRequest(HTTPRequest& request);

	private:
		std::string _version;
		std::string _statusMessage;
		std::string _statusLine;
		bool _keepAlive;
		size_t _contentLength;
		std::map<std::string, std::string> _headers;
		std::string _body;
		HTTPStatusCode _statusCode;

		std::map<HTTPStatusCode, std::string> _statusMessageMap;

		std::string message;


};

#endif