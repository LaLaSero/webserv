#include "ParseRequest.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include <iostream>
#include <cstring>
#include "../server/ServerException.hpp"

int main()
{
	char buffer[] = "POST /submit-form HTTP/1.1\r\nHost: example.com\r\nContent-Length: 11\r\nUser-Agent: TestAgent/1.0\r\n\r\nHello World";

	HTTPRequest request;
	ParseRequest parser(request);

	std::cout << std::endl;
	try
	{
		parser.parse(buffer);
		if (parser.isFinished())
			std::cout << "Request" << std::endl;
		request.print();
	} catch (const ServerException& e) {
		std::cerr << "ServerException: " << e.what() << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Response" << std::endl;

	HTTPResponse response;
	response.handleNormalRequest(request);

	return 0;
}
