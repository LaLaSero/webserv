#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstdio>
#include<map>


enum HTTPStatusCode
{
	// 100~ : informational response
	STATUS_100 = 100, // Continue
	STATUS_101 = 101, // Switching Protocols
	STATUS_102 = 102, // Processing
	STATUS_103 = 103, // Early Hints
	// 200~ : success response
	STATUS_200 = 200, // OK
	STATUS_201 = 201, // Created
	STATUS_202 = 202, // Accepted
	STATUS_203 = 203, // Non-Authoritative Information
	STATUS_204 = 204, // No Content
	STATUS_205 = 205, // Reset Content
	STATUS_206 = 206, // Partial Content
	STATUS_207 = 207, // Multi-Status
	STATUS_208 = 208, // Already Reported
	STATUS_226 = 226, // IM Used
	// 300~ : redirection response
	STATUS_300 = 300, // Multiple Choice
	STATUS_301 = 301, // Moved Permanently
	STATUS_302 = 302, // Found
	STATUS_303 = 303, // See Other
	STATUS_304 = 304, // Not Modified
	STATUS_305 = 305, // Use Proxy (not recommended)
	STATUS_306 = 306, // (unused)
	STATUS_307 = 307, // Temporary Redirect
	STATUS_308 = 308, // Permanent Redirect
	// 400~ : client error response
	STATUS_400 = 400, // Bad Request
	STATUS_401 = 401, // Unauthorized
	STATUS_402 = 402, // Payment Required
	STATUS_403 = 403, // Forbidden
	STATUS_404 = 404, // Not Found
	STATUS_405 = 405, // Method Not Allowed
	STATUS_406 = 406, // Not Acceptable
	STATUS_407 = 407, // Proxy Authentication Required
	STATUS_408 = 408, // Request Timeout
	STATUS_409 = 409, // Conflict
	STATUS_410 = 410, // Gone
	STATUS_411 = 411, // Length Required
	STATUS_412 = 412, // Precondition Failed
	STATUS_413 = 413, // Payload Too Large
	STATUS_414 = 414, // URI Too Long
	STATUS_415 = 415, // Unsupported Media Type
	STATUS_416 = 416, // Range Not Satisfiable
	STATUS_417 = 417, // Expectation Failed
	STATUS_418 = 418, // I'm a teapot
	STATUS_421 = 421, // Misdirected Request
	STATUS_422 = 422, // Unprocessable Entity
	STATUS_423 = 423, // Locked
	STATUS_424 = 424, // Failed Dependency
	STATUS_425 = 425, // Too Early
	STATUS_426 = 426, // Upgrade Required
	STATUS_428 = 428, // Precondition Required
	STATUS_429 = 429, // Too Many Requests
	STATUS_431 = 431, // Request Header Fields Too Large
	STATUS_451 = 451, // Unavailable For Legal Reasons
	// 500~ : server error response
	STATUS_500 = 500, // Internal Server Error
	STATUS_501 = 501, // Not Implemented
	STATUS_502 = 502, // Bad Gateway
	STATUS_503 = 503, // Service Unavailable
	STATUS_504 = 504, // Gateway Timeout
	STATUS_505 = 505, // HTTP Version Not Supported
	STATUS_506 = 506, // Variant Also Negotiates
	STATUS_507 = 507, // Insufficient Storage
	STATUS_508 = 508, // Loop Detected
	STATUS_510 = 510, // Not Extended
	STATUS_511 = 511 // Network Authentication Required
};
class HTTPRequest
{
    private:
        std::string    uri_;
    public:
        std::string get_Uri();
};

class HTTPResponse
{
    private:
		std::string		_version;
		std::string		_statusMessage;
		std::string		_statusLine;
		bool			_keepAlive;
		size_t			_contentLength;
		std::string		_body;
		std::string		message;

		HTTPStatusCode	_statusCode;

		std::map<std::string, std::string> 		_headers;
		std::map<HTTPStatusCode, std::string>	_statusMessageMap;
    public:
    	void setHeadersContentType(std::string& true_path);
    	void makeBodyGET(HTTPRequest &request);
    	void makeBodyPOST(HTTPRequest &request);
    	void makeBodyDELETE(HTTPRequest &request);
};


// ファイルを読み込んでその内容を返す関数
std::string get_file_content(const std::string& path) 
{
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file) 
        return "";
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::string HTTPRequest::get_Uri()
{
    return (uri_);
}

std::string make_true_path(std::string &uri)
{
    return (uri);
}

std::string get_forbidden_page()
{
    std::string body;
    body = "<html><head><title>403 Forbidden</title></head><body><h1>403 Forbidden</h1></body></html>";
    return (body);
}

std::string get_not_found_page()
{
    std::string body;
    body = "<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";
    return (body);
}

std::string get_file_extension(std::string& file_path)
{
    size_t dot_pos = file_path.find_last_of(".");
    if (dot_pos != std::string::npos) {
        return file_path.substr(dot_pos + 1);  // 拡張子部分を返す
    }
    return "";  // 拡張子がない場合
}

void HTTPResponse::setHeadersContentType(std::string& true_path)
{
    // 拡張子を取得
    std::string extension = get_file_extension(true_path);

    // 拡張子に基づいてContent-Typeを設定
    if (extension == "html") 
        _headers["Content-Type"] = "text/html";
    else if (extension == "jpg" || extension == "jpeg")
        _headers["Content-Type"] = "image/jpeg";
    else if (extension == "png")
        _headers["Content-Type"] = "image/png";
    else if (extension == "txt") 
        _headers["Content-Type"] = "text/plain";
    else if (extension == "json")
        _headers["Content-Type"] = "application/json";
    else if (extension == "pdf") 
        _headers["Content-Type"] = "application/pdf";
    else if (extension == "css") 
        _headers["Content-Type"] = "text/css";
    else if (extension == "js") 
        _headers["Content-Type"] = "application/javascript";
    else if (extension == "xml")
        _headers["Content-Type"] = "application/xml";
    else if (extension == "gif") 
        _headers["Content-Type"] = "image/gif";
    else if (extension == "svg") 
        _headers["Content-Type"] = "image/svg+xml";
    else if (extension == "")
        _headers["Content-Type"] = "application/octet-stream";
    else 
        _headers["Content-Type"] = "application/octet-stream";
}

void HTTPResponse::makeBodyGET(HTTPRequest &request)
{
    std::string uri = request.get_Uri();
    std::string true_path = make_true_path(uri);
    std::string body = get_file_content(true_path);
    if (body.empty())
    {
        if (access(true_path.c_str(), F_OK) == 0)
		{
            _statusCode = STATUS_403;
			_body = get_forbidden_page();
		}
        else
		{
            _statusCode = STATUS_404;
			_body = get_not_found_page();
		}
        _contentLength = _body.size();
    }
    else
    {
        _statusCode = STATUS_200;
        _body = body;
        _contentLength = _body.size();
        setHeadersContentType(true_path);
    }
}


void HTTPResponse::makeBodyPOST(HTTPRequest &request)
{
    std::string uri = request.get_Uri();
    std::string true_path = make_true_path(uri);
}

bool deleteFile(const char* filename) 
{
    if (remove(filename) == 0)
        return (true);
	else 
        return (false);
}


void HTTPResponse::makeBodyDELETE(HTTPRequest &request)
{
    std::string uri = request.get_Uri();
    std::string true_path = make_true_path(uri);

	if (deleteFile(true_path.c_str()))
	{
        _statusCode = STATUS_200;
		//if delete success no message in body;
		_contentLength = 0;
	}
	else
	{
        _statusCode = STATUS_500;
		_body = "<html><head><title>500 Internal Server Error</title></head><body><h1>500 Internal Server Error</h1></body></html>";
		_contentLength = _body.size();
	}
}