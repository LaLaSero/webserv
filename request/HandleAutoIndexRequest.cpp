#include "HTTPResponse.hpp"

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


std::vector<std::string> HTTPResponse::readDirectoryContents(std::string path)
{
	(void)path;
	std::vector<std::string> fileList;
	// ディレクトリの中身を取得
	return fileList;
}


std::string HTTPResponse::mapUriToPath(std::string uri)
{
	(void)uri;
	std::string path;
	// URIをファイルパスに変換
	return path;
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