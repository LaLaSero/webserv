#include "HTTPResponse.hpp"
#include "../server/ServerException.hpp"
#include "../config/Location.hpp"

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
	message.clear();
}

std::string HTTPResponse::getCurrentTime()
{
	std::time_t now = std::time(0);
	std::tm* now_tm = std::gmtime(&now);

	char buffer[80];
	strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", now_tm);

	return std::string(buffer);
}

void HTTPResponse::generateErrorResponse(HTTPStatusCode statusCode, const std::string& reasonPhrase, const std::string& message)
{
	_statusCode = statusCode;
	_statusMessage = reasonPhrase;
	_body = "<html><head><title>" + std::to_string(statusCode) + " " + reasonPhrase + "</title></head>"
			"<body><h1>" + std::to_string(statusCode) + " " + reasonPhrase + "</h1>"
			"<p>" + message + "</p></body></html>";
	makeMessage();
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
	setHeader("Date", getCurrentTime());
	setHeader("Server", SERVER_NAME);
	setHeader("Content-Type", "text/html");
	setHeader("Content-Length", std::to_string(_body.size()));
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
		message += it->first + ": " + it->second + "\r\n";
	message += "\r\n";
	if (_body.size() > 0)
		message += _body + "\r\n";
}

bool HTTPResponse::isCGIRequest(HTTPRequest& request)
{
	std::string uri = request.getUri();
	if (uri.find("/cgi-bin/") == 0)
	{
		return true;
	}
	return false;
}

bool HTTPResponse::isRedirectRequest(HTTPRequest& request)
{
	std::string uri = request.getUri();
	if (uri.find("/redirect/") == 0)
	{
		return true;
	}
	return false;
}

bool HTTPResponse::isAutoIndex(HTTPRequest& request)
{
	(void)request;
	// serverConfigでautoIndexが有効になっているかどうか
	return false;
}

// CGIに飛ばすか，リダイレクトか，ノーマルのレスポンスかを判別する
void HTTPResponse::selectResponseMode(HTTPRequest& request)
{
	std::string uri = request.getUri();

	if (isCGIRequest(request))
	{
		request.setMode(CGI_RESPONSE);
		handleCGIRequest(request);
	}
	else if (isRedirectRequest(request))
	{
		request.setMode(REDIRECT_RESPONSE);
		handleRedirectRequest(request);
	}
	else
	{
		request.setMode(NORMAL_RESPONSE);
		if (isAutoIndex(request))
		{
			// 自動インデックスの処理
			handleAutoIndex(request);
		}
		else
		{
			handleNormalRequest(request);
		}
	}

}