#include "HTTPResponse.hpp"
#include "HTTPRequest.hpp"
#include"MethodUtils.hpp"

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
		_body = get_error_page(_statusCode, _server, EP404);
        _contentLength = _body.size();
		return ;
	}
	if (is_dir(true_path))
	{
        if (find_location(_server,request.getPath()) == _server->getLocations().end())
        {
            _statusCode = STATUS_403;
		    _body = get_error_page(_statusCode, _server, EP403);
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
		            _body = get_error_page(_statusCode, _server, EP403);
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
		            _body = get_error_page(_statusCode, _server, EP404);
                    _contentLength = _body.size();
		            return ;
                }
                std::string body = get_file_content(true_path);
                if (body.empty())
                {
                    _statusCode = STATUS_403;
		            _body = get_error_page(_statusCode, _server, EP403);
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
		_body = get_error_page(_statusCode, _server, EP403);
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
