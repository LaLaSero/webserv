#include "ServerException.hpp"

// ServerException::ServerException(int statusCode, const std::string& message): _statusCode(statusCode), _message(message)
// {
// 	_fullMessage = "HTTP Error " + std::to_string(_statusCode) + ": " + _message;
// }

// ServerException::~ServerException() {}

// int ServerException::getStatusCode() const
// {
// 	return _statusCode;
// }

// const std::string& ServerException::getMessage() const
// {
// 	return _message;
// }

// const std::unordered_map<std::string, std::string>& ServerException::getHeaders() const
// {
// 	return _headers;
// }

// void ServerException::addHeader(const std::string& key, const std::string& value)
// {
// 	_headers[key] = value;
// }

// const char* ServerException::what() const
// {
// 	return _fullMessage.c_str();
// }
