#include "HTTPResponse.hpp"
#include "../server/ServerException.hpp"
#include "../config/Location.hpp"
#include <sstream>

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

void HTTPResponse::generateErrorResponse(HTTPStatusCode statusCode, const std::string& reasonPhrase, const std::string& message)
{
	_statusCode = statusCode;
	_statusMessage = reasonPhrase;
	std::stringstream ss;
	ss << "<html><head><title>" << statusCode << " " + reasonPhrase << "</title></head>"
			"<body><h1>" << statusCode << " " << reasonPhrase << "</h1>"
			"<p>" << message << "</p></body></html>";
	_body = ss.str();
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
	std::stringstream ss;

	ss  << _version << " " << _statusCode << " " << _statusMessageMap[_statusCode] << "\r\n";
	_statusLine = ss.str();
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
	std::stringstream ss;
	ss << _body.size();
	setHeader("Content-Length", ss.str());
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


// CGIに飛ばすか，リダイレクトか，ノーマルのレスポンスかを判別する
void HTTPResponse::selectResponseMode(HTTPRequest& request)
{
	std::string uri = request.getUri();

	if (request.get_errorno_() != 0)
	{
		makeBodyError(request);
	}
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

