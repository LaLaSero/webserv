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
		_statusCode = STATUS_501;
		_body = "<html><head><title>501 Not Implemented</title></head><body><h1>501 Not Implemented</h1><p>Method Not Implemented</p></body></html>";
	}
	makeMessage();
	_keepAlive = request.getHeader("Connection") == "keep-alive";
	std::cout << message << std::endl;

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

void HTTPResponse::handleCGIRequest(HTTPRequest& request)
{
	(void)request;
	// Location location;
	// location = request.getLocation();
	// CGIの処理
}

void HTTPResponse::handleRedirectRequest(HTTPRequest& request)
{
	(void)request;
	// Location location;
	// リダイレクトの処理
}

void HTTPResponse::makeBodyAutoIndex(std::string files, std::string uri, std::stringstream& ss)
{
	// Location location;
	// 自動インデックスの処理
}

bool HTTPResponse::isDirectoryRequest(HTTPRequest& request)
{
	// (void)request;
	// リクエストされたURIがディレクトリかどうか
	return false;
}

bool HTTPResponse::hasTrailingSlash(const std::string& uri) const
{
	return !uri.empty() && uri.back() == '/';
}

void HTTPResponse::redirectToTrailingSlash(const HTTPRequest& request)
{
	std::string uri = request.getUri();
	if (uri.back() != '/') {
		uri += "/";
	}
	_statusCode = STATUS_301;
	_headers["Location"] = uri;
	_body = "<html><head><title>301 Moved Permanently</title></head>"
			"<body><h1>301 Moved Permanently</h1>"
			"<p>Redirecting to " + uri + "</p></body></html>";
	makeMessage();
	std::cout << message << std::endl;
}


bool HTTPResponse::indexFileExist(HTTPRequest& request)
{
	(void)request;
	// indexファイルが存在するかどうか
	return false;
}

bool HTTPResponse::isAutoIndexEnabled(HTTPRequest& request)
{
	(void)request;
	// serverConfigでautoIndexが有効になっているかどうか
	return false;
}

std::vector<std::string> HTTPResponse::readDirectoryContents(std::string path)
{
	(void)path;
	std::vector<std::string> fileList;
	// ディレクトリの中身を取得
	return fileList;
}

std::string HTTPResponse::generateAutoIndexHTML(std::vector<std::string> fileList, std::string uri)
{
	(void)fileList;
	(void)uri;
	std::string htmlContent;
	// 自動インデックスのHTMLを生成
	return htmlContent;
}

void HTTPResponse::serveAutoIndex(std::string htmlContent, HTTPRequest& request)
{
	(void)htmlContent;
	(void)request;
	// 自動インデックスのレスポンスを返す
}

std::string HTTPResponse::mapUriToPath(std::string uri)
{
	(void)uri;
	std::string path;
	// URIをファイルパスに変換
	return path;
}

void HTTPResponse::handleAutoIndex(HTTPRequest& request)
{
	std::string uri = request.getUri();
	std::string path = mapUriToPath(uri);
	if(!isDirectoryRequest(request))
	{
		// normalリクエストに飛ばす
		handleNormalRequest(request);
		return ;
	}
	if (!hasTrailingSlash(uri))
	{
		redirectToTrailingSlash(request);
		return ;
	}
	if (indexFileExist(request))
	{
		handleNormalRequest(request);
		return ;
	}
	 if (!isAutoIndexEnabled(request))
	{
		_statusCode = STATUS_403;
		_body = "<html><head><title>403 Forbidden</title></head>"
				"<body><h1>403 Forbidden</h1><p>Directory listing is forbidden.</p></body></html>";
		makeMessage();
		std::cout << message << std::endl;
		return ;
	}

	// 自動インデックスの処理
	std::vector<std::string> fileList = readDirectoryContents(path);
	std::string htmlContent = generateAutoIndexHTML(fileList, uri);
	serveAutoIndex(htmlContent, request);

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