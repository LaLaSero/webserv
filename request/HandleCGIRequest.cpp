#include "HTTPResponse.hpp"
#include "../cgi/CgiHandler.hpp"

void HTTPResponse::handleCGIRequest(HTTPRequest& request)
{
	CgiHandler cgi_handler(request);

	std::string response;
	bool local_redirect_flag = cgi_handler.ExecuteCGI(response);

	std::cout << "CGI Response:\n\n" << response << std::endl;

	if (local_redirect_flag)
	{
		std::cout << "Local Redirect detected." << std::endl;
		// HTTPRequest redirect_request;
		// ParseRequest parser_request(redirect_request);
		// parser_request.parse(const_cast<char *>(response.c_str()));
		// std::cout << "Redirect Location: " << redirect_request.getUri() << std::endl;
	}

}
