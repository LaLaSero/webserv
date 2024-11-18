#include "HTTPResponse.hpp"

void HTTPResponse::makeBodyDELETE(HTTPRequest& request)
{
	(void)request;
	_body = "<html><head><title>200 OK</title></head><body><h1>200 OK</h1><p>DELETE request received!</p></body></html>";
}