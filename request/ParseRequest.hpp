#ifndef PARSEREQUEST_HPP
#define PARSEREQUEST_HPP

#include "HTTPRequest.hpp"
#include "parse_state/IParseState.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include "../server/ServerException.hpp"

class ParseRequest
{
	public:
		ParseRequest(HTTPRequest &request);
		~ParseRequest();

		void clear();
		void parse(char *buffer);

		bool readRequestLine(std::stringstream &ss);
		bool readHeaders(std::stringstream &ss);
		bool readBody(std::stringstream &ss);
		void finalize();

		bool hasBody() const { return _isChunked || _contentLength > 0; }
		HTTPRequest &getRequest() const;

		bool isFinished() const;
		void setFinished(bool finished);

	private:
		HTTPRequest &_request;
		bool _isChunked;
		size_t _contentLength;

		IParseState* _currentState;

		size_t _chunkSize;
		bool _isChunkSizeExpected;

		int getlineWithCRLF(std::stringstream &ss, std::string &line);
		std::vector<std::string> split(std::string str, char del);
		std::vector<std::string> split(std::string str, std::string del);
		int countSpace(const std::string &str);
		void searchLocation();

		bool isHex(const std::string& str);
		int ft_stoi(const std::string& str);
		bool ft_isdigit(char c);

		bool _isFinished;
};

#endif
