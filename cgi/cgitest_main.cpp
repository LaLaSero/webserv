#include "CgiHandler.hpp"
// test用main関数 環境変数を設定してCGIを実行しレスポンスを表示

int main()
{
	HTTPRequest request;
	
	CgiHandler cgi_handler(request);

	std::string response = cgi_handler.ExecuteCGI();

	std::cout << "CGI Response:\n"
			  << response << std::endl;

	return 0;
}
