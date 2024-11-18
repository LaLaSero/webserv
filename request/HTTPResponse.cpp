#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
	: _version("HTTP/1.1")
	, _keepAlive(true)
	, _body("")
	, _statusCode(STATUS_200)
{
	setStatusMessageMap();
}

HTTPResponse::~HTTPResponse()
{
}

void HTTPResponse::clear()
{
	_version = "HTTP/1.1";
	_headers.clear();
	_keepAlive = true;
	_body.clear();
}

void HTTPResponse::setStatusMessage(const std::string& statusMessage)
{
	_statusMessage = statusMessage;
}

void HTTPResponse::setHeader(const std::string& key, const std::string& value)
{
	_headers[key] = value;
}

void HTTPResponse::setStatusline()
{
	_statusLine = _version + " " + std::to_string(_statusCode) + " " + _statusMessageMap[_statusCode] + "\r\n";
}

void HTTPResponse::makeMessage()
{
	setStatusline();
	message += _statusLine;
	if (_keepAlive)
		setHeader("Connection", "keep-alive");
	else
		setHeader("Connection", "close");
	setHeader("Date", "Sun, 18 Oct 2009 08:56:53 GMT");
	setHeader("Server", "Apache/2.2.14 (Win32)");
	setHeader("Content-Length", std::to_string(_body.size()));
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
		message += it->first + ": " + it->second + "\r\n";
	message += "\r\n";
	if (_body.size() > 0)
		message += _body + "\r\n";
}

void HTTPResponse::makeBodyGET(HTTPRequest& request)
{
	(void)request;
	_body = "<html><head><title>200 OK</title></head><body><h1>200 OK</h1><p>GET request received!</p></body></html>";
}

void HTTPResponse::makeBodyPOST(HTTPRequest& request)
{
	(void)request;
	_body = "<html><head><title>200 OK</title></head><body><h1>200 OK</h1><p>POST request received!</p></body></html>";
}

void HTTPResponse::makeBodyDELETE(HTTPRequest& request)
{
	(void)request;
	_body = "<html><head><title>200 OK</title></head><body><h1>200 OK</h1><p>DELETE request received!</p></body></html>";
}


void HTTPResponse::handleNormalRequest(HTTPRequest& request)
{
	std::string method = request.getMethod();
	if (method == "GET")
	{
		// GETリクエストの処理
		makeBodyGET(request);
	}
	else if (method == "POST")
	{
		// POSTリクエストの処理
		makeBodyPOST(request);
	}
	else if (method == "DELETE")
	{
		// DELETEリクエストの処理
		makeBodyDELETE(request);
	}
	else
	{
		// 未対応のメソッド
		std::cerr << "Unsupported method: " << method << std::endl;
		std::exit(1);
	}
	makeMessage();
	_keepAlive = request.getHeader("Connection") == "keep-alive";
	std::cout << message << std::endl;

}
