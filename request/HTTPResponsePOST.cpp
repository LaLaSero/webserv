#include "HTTPResponse.hpp"

void HTTPResponse::makeBodyPOST(HTTPRequest& request)
{
	(void)request;
	_body = "<html><head><title>200 OK</title></head><body><h1>200 OK</h1><p>POST request received!</p></body></html>";
}