#include "ParseRequest.hpp"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "../config/Config.hpp"
#include "../server/ServerException.hpp"
#include "../config/Parser.hpp"
#include <iostream>
#include <cstring>

int main()
{
	// 設定ファイルを読み込む
	Config config;
	Parser parser;

	try {
		parser.LoadFile("server.conf");         // 設定ファイルを読み込み
		config = parser.MakeConfig();          // Configオブジェクトを生成
	} catch (const ServerException& e) {
		std::cerr << "Failed to load configuration: " << e.what() << std::endl;
		return 1;
	} catch (const std::exception& e) {
		std::cerr << "General exception: " << e.what() << std::endl;
		return 1;
	}

	// 設定内容のデバッグ出力（オプション）
	config.displayConfig();

	// モックリクエスト（テスト用のリクエストデータ）
	char buffer[] = "POST /index.html HTTP/1.1\r\n"
					"Host: example.com\r\n"
					"Content-Length: 11\r\n"
					"User-Agent: TestAgent/1.0\r\n\r\n"
					"Hello World";

	HTTPRequest request;
	ParseRequest parser_request(request);

	try {
		parser_request.parse(buffer);           // リクエストを解析
		if (parser_request.isFinished()) {
			std::cout << "Request successfully parsed." << std::endl;
			request.print();                    // リクエスト内容を出力（デバッグ用）
		}
	} catch (const ServerException& e) {
		std::cerr << "Error parsing request: " << e.what() << std::endl;
		return 1;
	} catch (const std::exception& e) {
		std::cerr << "General exception: " << e.what() << std::endl;
		return 1;
	}

	// HTTPResponseを作成し、リクエストを処理
	HTTPResponse response(config);

	try {
		response.selectResponseMode(request);   // 適切なレスポンスモードを選択し、処理
	} catch (const ServerException& e) {
		std::cerr << "Error handling request: " << e.what() << std::endl;
		return 1;
	} catch (const std::exception& e) {
		std::cerr << "General exception: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}