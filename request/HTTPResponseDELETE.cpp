#include "HTTPResponse.hpp"
#include"MethodUtils.hpp"


bool deleteFile(const char* filename) 
{
    if (remove(filename) == 0)
        return (true);
	else 
        return (false);
}

void HTTPResponse::makeBodyDELETE(HTTPRequest& request)
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
	if (is_dir(true_path))
	{
		_statusCode = STATUS_400;
		_body = get_error_page(_statusCode, _server, EP400);
        _contentLength = _body.size();
	}
 	if (access(true_path.c_str(), F_OK) != 0)
	{
        _statusCode = STATUS_404;
		_body = get_error_page(_statusCode, _server, EP404);
        _contentLength = _body.size();
		return ;
	}
	if (deleteFile(true_path.c_str()))
    {
        _statusCode = STATUS_200;
        _contentLength = 0;
    }
	else
	{
        _statusCode = STATUS_500;
		_body = get_error_page(_statusCode, _server, EP500);
        _contentLength = _body.size();
	}
}