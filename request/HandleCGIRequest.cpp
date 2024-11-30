#include "HTTPResponse.hpp"
#include "../cgi/CgiHandler.hpp"

void HTTPResponse::handleCGIRequest(HTTPRequest& request)
{

	CgiHandler cgi_handler(request);

	std::string response = cgi_handler.ExecuteCGI();

	std::cout << "CGI Response:\n\n" << response << std::endl;
}
