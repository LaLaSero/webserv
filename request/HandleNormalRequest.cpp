#include "HTTPResponse.hpp"

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
		request.setMode(POST_RESPONSE);
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