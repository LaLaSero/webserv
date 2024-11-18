#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>

class HTTPRequest
{
	private:
		std::string _method;
		std::string _uri;
		std::string _version;
		std::string _path;
		std::string _query;
		std::string _host;
		std::string _port;
		std::map<std::string, std::string> _headers;
		std::string _body;
		std::string _location;

		int _mode;


	public:
		HTTPRequest();
		~HTTPRequest();

		const std::string& getMethod() const;
		void setMethod(const std::string& method);

		const std::string& getUri() const;
		void setUri(const std::string& uri);

		const std::string& getVersion() const;
		void setVersion(const std::string& version);

		const std::string& getPath() const;
		void setPath(const std::string& path);

		const std::string& getQuery() const;
		void setQuery(const std::string& query);

		const std::string& getHost() const;
		void setHost(const std::string& host);

		const std::string& getPort() const;
		void setPort(const std::string& port);

		const std::string& getLocation() const;
		void setLocation(const std::string& location);

		int getMode() const;
		void setMode(int mode);

		const std::map<std::string, std::string>& getHeaders() const;
		void setHeaders(const std::pair<std::string, std::string>& header);
		std::string getHeader(const std::string& key) const;

		const std::string& getBody() const;
		void setBody(const std::string& body);
		void addBody(const std::string& body);

		void clear();

		void print() const;

		HTTPRequest &getRequest() const;

};

#endif 