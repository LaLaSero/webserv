#include "HTTPResponse.hpp"
#include "HTTPRequest.hpp"
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
#include<sys/stat.h>
#include<map>


std::string get_file_content(const std::string& path) 
{
    std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
    if (!file) 
        return "";
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::string make_true_path(const std::string &uri, const std::string &root_path)
{
    std::string tmp;

    if (root_path.empty())
        return (uri);
    else
        tmp = root_path + uri;
    return (tmp);
}

// std::string get_forbidden_page(int status, const Server &serv)
// {
//     std::string body;
//     body = "<html><head><title>403 Forbidden</title></head><body><h1>403 Forbidden</h1></body></html>";
//     return (body);
// }

std::string get_forbidden_page(int status, const ChildServer *serv)
{
    std::string body;

    // errorPagesの検索
    std::map<int, std::string>::const_iterator it = serv->get_error_page().find(status);
    
    std::cout << status << std::endl;
    // statusに対応するエラーページが見つかった場合
    if (it != serv->get_error_page().end()) 
        body = get_file_content(it->second);
    else
        // 見つからない場合はデフォルトの403ページを設定
        body = "<html><head><title>403 Forbidden</title></head><body><h1>403 Forbidden</h1></body></html>";
    return body;
}


std::string get_not_found_page(int status, const ChildServer *serv)
{
    std::string body;

    // errorPagesの検索
    std::map<int, std::string>::const_iterator it = serv->get_error_page().find(status);
    
    std::cout << status << std::endl;
    // statusに対応するエラーページが見つかった場合
    if (it != serv->get_error_page().end()) 
        body = get_file_content(it->second);
    else
        // 見つからない場合はデフォルトの403ページを設定
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

static bool is_dir(const std::string &path)
{
	struct stat pathStat;
	if (stat(path.c_str(), &pathStat) == 0)
		return S_ISDIR(pathStat.st_mode);
	else
		return (false);
}

std::string get_redirect_body(const std::string uri)
{
    std::string html = "<html><body><p>You are being redirected to <a href=\"" + uri + "\">this page</a>.</p></body></html>";
    return (html);
}

std::vector<Location>::const_iterator find_location(const ChildServer *Server, const std::string& path)
{
    const std::vector<Location>& loc = Server->getLocations();
    
    for (std::vector<Location>::const_iterator it = loc.begin(); it != loc.end(); ++it)
    {
        if (path.compare(0, it->getPath().size(), it->getPath()) == 0) 
            return it;
    }
    return Server->getLocations().end();
}

void HTTPResponse::makeBodyGET(HTTPRequest& request)
{
	std::string uri = request.getUri();
    std::string true_path = uri;
    Location loc;
    if (!(_server->getLocations().empty()))
    {
      if (find_location(_server,request.getPath()) != _server->getLocations().end())
      {
        std::cout << (*(find_location(_server,request.getPath()))).getPath() << std::endl;
        loc = *(find_location(_server,request.getPath()));
        if (!loc.getRedirection().second.empty())
        {
            _statusCode = static_cast<HTTPStatusCode>(loc.getRedirection().first);
            _body = get_redirect_body(loc.getRedirection().second);
            return ;
        }
        true_path = make_true_path(uri, loc.getRootDirectory());
      }
    }
 	if (access(true_path.c_str(), F_OK) != 0)
	{
        _statusCode = STATUS_404;
		_body = get_not_found_page(_statusCode, _server);
        _contentLength = _body.size();
		return ;
	}
	if (is_dir(true_path))
	{
        if (find_location(_server,request.getPath()) == _server->getLocations().end())
        {
            _statusCode = STATUS_403;
		    _body = get_forbidden_page(_statusCode, _server);
            _contentLength = _body.size();
            return ;
        }
        else
        {
            if (loc.getDefaultFile().empty())
            {
                if(loc.isDirectoryListing())
                {
                    _statusCode = STATUS_200;
	                std::vector<FileInfo> fileList = readDirectoryContents(true_path);
	                 _body =  generateAutoIndexHTML(fileList, uri);
                    _contentLength = _body.size();
                    return ;
                }
                else
                {
                    _statusCode = STATUS_403;
		            _body = get_forbidden_page(_statusCode, _server);
                    _contentLength = _body.size();
                    return ;
                }
            }
            else
            {
                true_path = true_path + loc.getDefaultFile();
 	            if (access(true_path.c_str(), F_OK) != 0)
	            {
                    _statusCode = STATUS_404;
		            _body = get_not_found_page(_statusCode, _server);
                    _contentLength = _body.size();
		            return ;
                }
                std::string body = get_file_content(true_path);
                if (body.empty())
                {
                    _statusCode = STATUS_403;
		            _body = get_forbidden_page(_statusCode, _server);
                    _contentLength = _body.size();
                }
                else
                {
                    _statusCode = STATUS_200;
                    _body = body;
                    _contentLength = _body.size();
                    setHeadersContentType(true_path);
                    return;
                }
            }
            return ;
        }
	}
    std::string body = get_file_content(true_path);
    if (body.empty())
    {
        _statusCode = STATUS_403;
		_body = get_forbidden_page(_statusCode, _server);
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

std::string HTTPResponse::getStatusMessage()
{
    return (_statusLine);
}

std::string HTTPResponse::makeBodyResponse()
{
    std::string res;

    res += _statusLine;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) 
    {
		res += "  " + it->first + ": " + it->second  + "\r\n";
	}
    res += "\r\n";
    res += _body;
    return (res);
}
