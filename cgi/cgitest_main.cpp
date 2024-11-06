#include "CgiHandler.hpp"
// test用main関数 環境変数を設定してCGIを実行しレスポンスを表示

int main()
{
	std::map<std::string, std::string> env_vars;
	env_vars["REQUEST_METHOD"] = "GET";
	env_vars["QUERY_STRING"] = "name=Testuser";
	env_vars["SCRIPT_NAME"] = "./cgitest.py"; 
	env_vars["CONTENT_TYPE"] = "dummy";
	env_vars["CONTENT_LENGTH"] = "10";

	CgiHandler cgi_handler(env_vars.at("SCRIPT_NAME"), env_vars);

	std::string response = cgi_handler.ExecuteCGI();

	std::cout << "CGI Response:\n"
			  << response << std::endl;

	return 0;
}
