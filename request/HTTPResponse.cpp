#include "HTTPResponse.hpp"
#include "../server/ServerException.hpp"
#include "../config/Location.hpp"

HTTPResponse::HTTPResponse(const Config& _config)
	: _config(_config)
	, _version("HTTP/1.1")
	, _keepAlive(true)
	, _body("")
	, _statusCode(STATUS_200)
	,_server(NULL)
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

const std::string& HTTPResponse::getMessage() const
{
	return message;
}

void HTTPResponse::generateErrorResponse(HTTPStatusCode statusCode, const std::string& reasonPhrase, const std::string& message)
{
	std::ostringstream oss;
	oss << statusCode;

	std::string statusCodeStr = oss.str();

	_statusCode = statusCode;
	_statusMessage = reasonPhrase;
	_body = "<html><head><title>" + statusCodeStr + " " + reasonPhrase + "</title></head>"
			"<body><h1>" + statusCodeStr + " " + reasonPhrase + "</h1>"
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
	std::ostringstream oss;
	oss << _statusCode;

	std::string statusCodeStr = oss.str();
	_statusLine = _version + " " + statusCodeStr + " " + _statusMessageMap[_statusCode] + "\r\n";
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
	std::ostringstream oss;
	oss << _body.size();
	setHeader("Content-Length", oss.str());
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it)
		message += it->first + ": " + it->second + "\r\n";
	message += "\r\n";
	if (_body.size() > 0)
		message += _body + "\r\n";
}

bool HTTPResponse::isCGIRequest(HTTPRequest& request)
{
	std::string uri = request.getUri();
	if (uri.find("/cgi-bin/") != std::string::npos)
	{
		return true;
	}
	return false;
}

const std::string& HTTPResponse::getBody() const
{
	return _body;
}

bool HTTPResponse::isAutoIndex(HTTPRequest& request)
{
	// リクエストのURIを取得
	std::string uri = request.getUri();

	// ConfigからChildServerを取得
	const std::vector<ChildServer>& servers = _config.getchildserver();

	// 範囲ベースのforループをイテレータに置き換え
	std::vector<ChildServer>::const_iterator serverIt;
	for (serverIt = servers.begin(); serverIt != servers.end(); ++serverIt)
	{
		// 各サーバー内のLocationを探索
		const std::vector<Location>& locations = serverIt->getLocations();

		std::vector<Location>::const_iterator locationIt;
		for (locationIt = locations.begin(); locationIt != locations.end(); ++locationIt)
		{
			// URIがLocationのパスに一致するか確認
			if (uri.find(locationIt->getPath()) == 0) // 完全一致でも良い場合、修正する
			{
				// autoIndexの設定を確認
				return locationIt->isDirectoryListing();
			}
		}
	}

	// 一致するLocationが見つからなかった場合、falseを返す
	return false;
}

void HTTPResponse::set405Error(HTTPRequest& request)
{
	_statusCode = STATUS_405;
	_body = "<html><head><title>405 Method Not Allowed</title></head><body><h1>405 Method Not Allowed</h1><p>Method Not Allowed</p></body></html>";
	makeMessage();
	_keepAlive = request.getHeader("Connection") == "keep-alive";
	// std::cout << message << std::endl;
}

void HTTPResponse::set500Error()
{
	_statusCode = STATUS_500;
	_body = "<html><head><title>500 Internal Server Error</title></head><body><h1>500 Internal Server Error</h1><p>Internal Server Error</p></body></html>";
	makeMessage();
	_keepAlive = false;
	// std::cout << message << std::endl;
}

void HTTPResponse::setStatusCode(HTTPStatusCode statusCode)
{
	_statusCode = statusCode;
}

void HTTPResponse::setBody(const std::string& body)
{
	_body = body;
}

void HTTPResponse::setContentLength(size_t contentLength)
{
	_contentLength = contentLength;
}

// CGIに飛ばすか，リダイレクトか，ノーマルのレスポンスかを判別する
void HTTPResponse::selectResponseMode(HTTPRequest& request)
{
	std::string uri = request.getUri();
	
	// std::cout << uri << std::endl;

	if (request.get_errorno_() != 0)
	{
		makeBodyError(request);
	}
	if (isCGIRequest(request))
	{
		request.setMode(CGI_RESPONSE);
		// std::cout << "cgi" << std::endl;
		// handleCGIRequest(request);
	}
	else
	{
		request.setMode(NORMAL_RESPONSE);
		if (isAutoIndex(request))
			handleAutoIndex(request);
		else
			handleNormalRequest(request);
	}

}


std::string HTTPResponse::intToString(int number) const
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void HTTPResponse::SetChildServer(const ChildServer *cs)
{
	_server = cs;
}