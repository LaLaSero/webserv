#include "ParseRequest.hpp"
#include <iostream>
#include <cstring>
#include "../server/ServerException.hpp"

int main()
{
	char buffer[] = "POST /submit-form HTTP/1.1\r\nHost: example.com\r\nContent-Length: 11\r\nUser-Agent: TestAgent/1.0\r\n\r\nHello World";

	HTTPRequest request;
	ParseRequest parser(request);

	try
	{
		parser.parse(buffer);
		if (parser.isFinished())
			std::cout << "Request is finished" << std::endl;
		request.print();
	} catch (const ServerException& e) {
		std::cerr << "ServerException: " << e.what() << std::endl;
	}

	return 0;
}
