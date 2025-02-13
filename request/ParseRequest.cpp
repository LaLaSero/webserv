#include "ParseRequest.hpp"
#include "../server/ServerException.hpp"
#include "./parse_state/RequestLineState.hpp"
#include "./parse_state/HeadersState.hpp"
#include "./parse_state/BodyState.hpp"
#include "./parse_state/FinishState.hpp"
#include <cstdlib>

ParseRequest::ParseRequest(HTTPRequest &request): _request(request), _isChunked(false), _contentLength(0),
	_currentState(NULL), _chunkSize(0), _isChunkSizeExpected(false), _isFinished(false)
{
	_currentState = new RequestLineState();
}

ParseRequest::~ParseRequest()
{
	if (_currentState)
		delete _currentState;
}

void ParseRequest::clear()
{
	_isChunked = false;
	_contentLength = 0;
	_request.clear();
	if (_currentState)
		delete _currentState;
	_currentState = new RequestLineState();
	_chunkSize = 0;
	_isChunkSizeExpected = false;
}

HTTPRequest &ParseRequest::getRequest() const
{
	return _request;
}

void ParseRequest::parse(const char *buffer)
{
	std::stringstream bufferStream(buffer);

	while (_currentState)
	{
		IParseState* nextState = _currentState->handle(*this, bufferStream);
		if (nextState != _currentState)
		{
			delete _currentState;
			_currentState = nextState;
		}
		else
			break;
	}
}

bool ParseRequest::readRequestLine(std::stringstream &ss)
{
	std::string line;
	if (!getlineWithCRLF(ss, line)) {
		return false;
	}
	if (line.empty()) {
		// this->_request.set_errorno_(errorno_badreq);
		throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
		return false;
	}
	std::vector<std::string> request_line = split(line, ' ');
	if (countSpace(line) != 2)
	{
		// this->_request.set_errorno_(errorno_badreq);
		throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
		return false;
	}
	if (request_line.size() != 3)
	{
		// this->_request.set_errorno_(errorno_badreq);
		throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
		return false;
	}
	if (request_line[2] != "HTTP/1.1")
	{
		// this->_request.set_errorno_(errorno_notsup);
		throw ServerException(HTTP_HTTP_VERSION_NOT_SUPPORTED, "HTTP Version Not Supported");
		return false;
	}
	if (request_line[1][0] != '/' || request_line[1].find("/../") != std::string::npos)
	{
		// this->_request.set_errorno_(errorno_badreq);
		throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
		return false;
	}
	_request.setMethod(request_line[0]);
	_request.setUri(request_line[1]);
	_request.setVersion(request_line[2]);

	searchLocation();

	return true;
}

bool ParseRequest::readHeaders(std::stringstream &ss)
{
	std::string line;
	while (getlineWithCRLF(ss, line))
	{
		if (line.empty())
			return true;
		std::vector<std::string> header = split(line, ": ");
		if (header.size() != 2)
			throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
		std::pair<std::string, std::string> pair;
		pair.first = header[0];
		pair.second = header[1];

		if (header[0] == "Transfer-Encoding" && header[1] == "chunked")
			_isChunked = true;
		if (header[0] == "Content-Length") {
			if (!ft_isdigit(header[1][0]))
				throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
			int length = ft_stoi(header[1]);
			if (length < 0)
				throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
			_contentLength = length;
		}
		if (header[0] == "Host") {
			std::vector<std::string> host = split(header[1], ':');
			if (host.size() == 2) {
				_request.setHost(host[0]);
				int port = ft_stoi(host[1]);
				if (port < 0 || port > 65535)
					throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
				_request.setPort(host[1]);
			} else if (host.size() == 1) {
				_request.setHost(host[0]);
				_request.setPort("80");
			} else {
				throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
			}
			if (host.size() == 1)
				pair.second = host[0];
			else
				pair.second = header[1];
		}

		_request.setHeaders(pair);

		if (!_request.getHeader("Transfer-Encoding").empty() && !_request.getHeader("Content-Length").empty())
			throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
	}
	return false;
}

bool ParseRequest::readBody(std::stringstream &ss)
{
	if (_isChunked) {
		return false;
	} else {
		std::streamsize remaining = _contentLength - _request.getBody().size();
		if (remaining <= 0)
			return true;

		std::string bodyPart;
		bodyPart.resize(remaining);

		ss.read(&bodyPart[0], remaining);
		std::streamsize bytesRead = ss.gcount();
		bodyPart.resize(bytesRead);

		_request.addBody(bodyPart);

		if (_request.getBody().size() == static_cast<size_t>(_contentLength))
			return true;
		else
			return false;
	}
}

void ParseRequest::finalize()
{
	if (_request.getHeader("Host").empty())
		throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
	// std::cout << ">>>>>>>>>>>>finalize<<<<<<<<<<<<<" << std::endl;
	// std::cout << ">>>>>>>>>>>>_contentLength: " << _contentLength << "<<<<<<<<<<<<<" << std::endl;
	// std::cout << ">>>>>>>>>>>>_request.getBody().size(): " << _request.getBody().size() << "<<<<<<<<<<<<<" << std::endl;
	if (_contentLength > 0 && _request.getBody().size() < _contentLength)
		setFinished(false);
	if (_contentLength > 0 && _request.getBody().size() != _contentLength)
		throw ServerException(HTTP_BAD_REQUEST, "Bad Request");
	setFinished(true);
}

int ParseRequest::getlineWithCRLF(std::stringstream &ss, std::string &line)
{
	if (!std::getline(ss, line))
		return 0;
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	return 1;
}

std::vector<std::string> ParseRequest::split(std::string str, char del)
{
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string tmp;

	while (std::getline(ss, tmp, del)) {
		if (!tmp.empty())
			result.push_back(tmp);
	}
	return result;
}

std::vector<std::string> ParseRequest::split(std::string str, std::string del)
{
	std::vector<std::string> result;
	size_t pos = str.find(del);
	size_t del_size = del.size();

	if (pos == std::string::npos)
		return result;
	result.push_back(str.substr(0, pos));
	result.push_back(str.substr(pos + del_size));
	return result;
}

int ParseRequest::countSpace(const std::string &str)
{
	int count = 0;
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] == ' ')
			++count;
	}
	return count;
}

void ParseRequest::searchLocation()
{
	std::string uri = _request.getUri();
	if (uri == "/") {
		_request.setLocation("/");
		return;
	}
	std::vector<std::string> uri_split = split(uri, '/');
	if (uri_split.size() < 2 && uri[uri.size() - 1] != '/') {
		_request.setLocation("/");
		return;
	}
	std::string location = "/" + uri_split[0] + "/";
	_request.setLocation(location);
}

bool ParseRequest::isHex(const std::string& str)
{
	for (size_t i = 0; i < str.size(); ++i) {
		if (!std::isxdigit(static_cast<unsigned char>(str[i])))
			return false;
	}
	return true;
}

// size_t ParseRequest::getContentLength() const
// {
// 	return _contentLength;
// }

// const std::string& ParseRequest::getBody() const
// {
// 	return _request.getBody();
// }

int ParseRequest::ft_stoi(const std::string& str)
{
	return std::atoi(str.c_str());
}

bool ParseRequest::ft_isdigit(char c) {
	return std::isdigit(static_cast<unsigned char>(c));
}

bool ParseRequest::isFinished() const
{
	return _isFinished;
}

void ParseRequest::setFinished(bool finished)
{
	_isFinished = finished;
}
