/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponsePOST.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:58:26 by ryanagit          #+#    #+#             */
/*   Updated: 2024/12/01 20:41:55 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"
#include "HTTPRequest.hpp"
#include"MethodUtils.hpp"

void HTTPResponse::HelpPostStatusChange(HTTPStatusCode i)
{
  if (i == STATUS_200)
  {
    _statusCode = i;
    _contentLength = _body.size();
  }
  else
  {
    _statusCode = i;
		_body = get_error_page(_statusCode, _server, EP500);
    _contentLength = _body.size();
  }
}

bool HTTPResponse::makeBodyPOST(HTTPRequest& request)
{
	std::string uri = request.getUri();
  std::string true_path = uri;
  Location loc;

  if (is_max_over(request.getBody(), _server->get_request_max()))
  {
    _statusCode = STATUS_400;
		_body = get_error_page(_statusCode, _server, EP400);
    _contentLength = _body.size();
		return (true);
  }
  if (!(_server->getLocations().empty()))
  {
    if (find_location(_server,request.getPath()) != _server->getLocations().end())
    {
      loc = *(find_location(_server,request.getPath()));
      if (!loc.getRedirection().second.empty())
      {
        _statusCode = static_cast<HTTPStatusCode>(loc.getRedirection().first);
        _body = get_redirect_body(loc.getRedirection().second);
        _contentLength = _body.size();
        return (true);
      }
      true_path = make_true_path(uri, loc.getRootDirectory());
      if (!(loc.getUploadDirectory().empty()))
        true_path = loc.getUploadDirectory();
    }
  }
  std::cout << true_path << std::endl;
 	if (access(true_path.c_str(), F_OK) != 0)
	{
    _statusCode = STATUS_404;
		_body = get_error_page(_statusCode, _server, EP404);
    _contentLength = _body.size();
		return (true);
	}
  std::string file_path = true_path + "posted.txt";
 	if (access(file_path.c_str(), F_OK) == 0)
	{
    _statusCode = STATUS_200;
		return (true);
	}
  return (false);
}