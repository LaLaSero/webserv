#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP

#include <exception>
#include <string>
// #include <unordered_map>
#include <sstream>
#include <map>

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
		ServerException(int statusCode, const std::string& message)
			: _statusCode(statusCode), _message(message)
		{
			std::stringstream ss;
			ss << "HTTP Error " << _statusCode << ": " << _message;
			_fullMessage = ss.str();
		}

		virtual ~ServerException() throw() {}

		int getStatusCode() const
		{
			return _statusCode;
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
		// std::unordered_map<std::string, std::string> _headers;
		std::map<std::string, std::string> _headers;

};

#endif
