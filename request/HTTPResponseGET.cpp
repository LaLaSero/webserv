#include "HTTPResponse.hpp"

void HTTPResponse::makeBodyGET(HTTPRequest& request)
{
	(void)request;
	_body = "<html><head><title>200 OK</title></head><body><h1>200 OK</h1><p>GET request received!</p></body></html>";
}
