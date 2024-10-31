#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse()
	: _version("HTTP/1.1"), _body(""), _statusMessage("OK"), _keepAlive(true)
{
}

HTTPResponse::~HTTPResponse()
{
}

// レスポンスをクリアする
void
HTTPResponse::clear()
{
	_version = "HTTP/1.1";
	_headers.clear();
	_body.clear();
	_statusMessage = "OK";
	_keepAlive = true;
}


