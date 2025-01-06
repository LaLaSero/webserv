#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP

#include <exception>
#include <string>
// #include <unordered_map>
#include <sstream>
#include <map>
#include <iostream>

#define HTTP_OK 200
#define HTTP_BAD_REQUEST 400
#define HTTP_NOT_FOUND 404
#define HTTP_METHOD_NOT_ALLOWED 405
#define HTTP_REQUEST_TIMEOUT 408
#define HTTP_PAYLOAD_TOO_LARGE 413
#define HTTP_INTERNAL_SERVER_ERROR 500
#define HTTP_NOT_IMPLEMENTED 501
#define HTTP_HTTP_VERSION_NOT_SUPPORTED 505

class ServerException : public std::exception 
{
	public:
		ServerException(int statusCode, const std::string& message): _statusCode(statusCode), _message(message)
		{
			_fullMessage = createErrorResponse(statusCode, message);
		}

		virtual ~ServerException() throw() {}

		int getStatusCode() const
		{
			return _statusCode;
		}

		static std::string getReasonPhrase(int statusCode)
		{
			switch (statusCode) {
				case 400: return "Bad Request";
				case 404: return "Not Found";
				case 405: return "Method Not Allowed";
				case 413: return "Payload Too Large";
				case 500: return "Internal Server Error";
				case 505: return "HTTP Version Not Supported";
				default:  return "Unknown";
			}
		}


		std::string createErrorResponse(int statusCode, const std::string& message)
		{
			// HTMLのボディを作成（簡易例）
			// 実際には独自のカスタムエラーページなどを返すことが多い
			std::ostringstream bodyStream;
			bodyStream << "<html>\n"
					<< "<head><title>" << statusCode << " " << getReasonPhrase(statusCode) << "</title></head>\n"
					<< "<body>\n"
					<< "<h1>" << statusCode << " " << getReasonPhrase(statusCode) << "</h1>\n"
					<< "<p>" << message << "</p>\n"
					<< "</body></html>\n";
			std::string body = bodyStream.str();
			
			// レスポンス全体を組み立て
			std::ostringstream responseStream;
			responseStream << "HTTP/1.1 " << statusCode << " " << getReasonPhrase(statusCode) << "\r\n"
						<< "Content-Type: text/html\r\n"
						<< "Content-Length: " << body.size() << "\r\n"
						<< "Connection: close\r\n"
						<< "\r\n"
						<< body;
			return responseStream.str();
		}

		const std::string& getMessage() const
		{
			return _message;
		}

		const std::map<std::string, std::string>& getHeaders() const
		{
			return _headers;
		}
		void addHeader(const std::string& key, const std::string& value)
		{
			_headers[key] = value;
		}
		virtual const char* what() const throw()
		{
			return _fullMessage.c_str();
		}

	private:
		int _statusCode;
		std::string _message;
		std::string _fullMessage;
		std::map<std::string, std::string> _headers;

};

#endif
