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

		void generateErrorResponse(HTTPStatusCode statusCode, const std::string& reasonPhrase, const std::string& message);

		void selectResponseMode(HTTPRequest& request);
		void setStatusMessageMap(void);
	
		void handleNormalRequest(HTTPRequest& request);

		void makeBodyGET(HTTPRequest& request);
		void makeBodyPOST(HTTPRequest& request);
		void makeBodyDELETE(HTTPRequest& request);

		bool isCGIRequest(HTTPRequest& request);
		void handleCGIRequest(HTTPRequest& request);

		bool isRedirectRequest(HTTPRequest& request);
		void handleRedirectRequest(HTTPRequest& request);

		bool isAutoIndex(HTTPRequest& request);
		void handleAutoIndex(HTTPRequest& request);
		void makeBodyAutoIndex(std::string files, std::string uri, std::stringstream& ss);

		bool isDirectoryRequest(HTTPRequest& request);
		bool hasTrailingSlash(const std::string& uri) const;
		void redirectToTrailingSlash(const HTTPRequest& request);

		bool indexFileExist(HTTPRequest& request);
		bool isAutoIndexEnabled(HTTPRequest& request);

		std::vector<std::string> readDirectoryContents(std::string path);
		std::string generateAutoIndexHTML(std::vector<std::string> fileList, std::string uri);
		void serveAutoIndex(std::string htmlContent, HTTPRequest& request);
		std::string mapUriToPath(std::string uri);

	private:
		std::string		_version;
		std::string		_statusMessage;
		std::string		_statusLine;
		bool			_keepAlive;
		size_t			_contentLength;
		std::string		_body;
		std::string		message;

		HTTPStatusCode	_statusCode;

		std::map<std::string, std::string> 		_headers;
		std::map<HTTPStatusCode, std::string>	_statusMessageMap;

};

#endif