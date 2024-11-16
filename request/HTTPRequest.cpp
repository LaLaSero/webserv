#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest()
	: _method(""), _uri(""), _version(""), _path(""), _query(""), _host(""), _port("80"), _body("")
{
}

HTTPRequest::~HTTPRequest()
{
}

const std::string& HTTPRequest::getMethod() const
{
	return _method;
}

void HTTPRequest::setMethod(const std::string& method)
{
	_method = method;
}

const std::string& HTTPRequest::getUri() const
{
	return _uri;
}

void HTTPRequest::setUri(const std::string& uri)
{
	_uri = uri;
	// URIからパスとクエリを分離
	size_t pos = uri.find('?');
	if (pos != std::string::npos) {
	_path = uri.substr(0, pos);
	_query = uri.substr(pos + 1);
	} else {
	_path = uri;
	_query = "";
	}
}

const std::string& HTTPRequest::getVersion() const
{
	return _version;
}

void HTTPRequest::setVersion(const std::string& version)
{
	_version = version;
}

const std::string& HTTPRequest::getPath() const
{
	return _path;
}

void HTTPRequest::setPath(const std::string& path)
{
	_path = path;
}

const std::string& HTTPRequest::getQuery() const
{
	return _query;
}

void HTTPRequest::setQuery(const std::string& query)
{
	_query = query;
}

const std::string& HTTPRequest::getHost() const
{
	return _host;
}

void HTTPRequest::setHost(const std::string& host)
{
	_host = host;
}

const std::string& HTTPRequest::getPort() const
{
	return _port;
}

void HTTPRequest::setPort(const std::string& port)
{
	_port = port;
}

void HTTPRequest::setLocation(const std::string& location)
{
	_location = location;
}

const std::map<std::string, std::string>& HTTPRequest::getHeaders() const
{
	return _headers;
}

void HTTPRequest::setHeaders(const std::pair<std::string, std::string>& header)
{
	_headers[header.first] = header.second;
}

std::string HTTPRequest::getHeader(const std::string& key) const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find(key);
	if (it != _headers.end())
		return it->second;
	return "";
}

const std::string& HTTPRequest::getBody() const
{
	return _body;
}

void HTTPRequest::setBody(const std::string& body)
{
	_body = body;
}

void HTTPRequest::addBody(const std::string& body)
{
	_body += body;
}

void HTTPRequest::clear()
{
	_method.clear();
	_uri.clear();
	_version.clear();
	_path.clear();
	_query.clear();
	_host.clear();
	_port = "80";
	_headers.clear();
	_body.clear();
}

void HTTPRequest::print() const
{
	std::cout << "Method: " << _method << std::endl;
	std::cout << "URI: " << _uri << std::endl;
	std::cout << "Version: " << _version << std::endl;
	std::cout << "Path: " << _path << std::endl;
	std::cout << "Query: " << _query << std::endl;
	std::cout << "Host: " << _host << std::endl;
	std::cout << "Port: " << _port << std::endl;
	std::cout << "Headers:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
		std::cout << "  " << it->first << ": " << it->second << std::endl;
	}
	std::cout << "Body: " << _body << std::endl;
}
