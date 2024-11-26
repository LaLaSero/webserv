/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponsePOST.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:58:26 by ryanagit          #+#    #+#             */
/*   Updated: 2024/11/26 15:41:46 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"
#include "HTTPRequest.hpp"
#include"MethodUtils.hpp"

bool try_makefile(const std::string &body, const std::string dir_path)
{
    std::string file_data = body;
    std::string save_path = dir_path + "posted.txt";

    std::ofstream file(save_path, std::ios::binary);
    if (!file.is_open())
        return (false);
    file.write(file_data.c_str(), file_data.size());
    file.close();
    if (!file)
        return (false);
    return (true);
}

void HTTPResponse::makeBodyPOST(HTTPRequest& request)
{
	std::string uri = request.getUri();
  std::string true_path = uri;
  Location loc;

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
        return ;
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
		return ;
	}
  std::string file_path = true_path + "posted.txt";
 	if (access(file_path.c_str(), F_OK) == 0)
	{
    _statusCode = STATUS_200;
		return ;
	}
  if (try_makefile(request.getBody(),true_path))
  {
    _statusCode = STATUS_201;
    _contentLength = _body.size();
  }
  else
  {
    _statusCode = STATUS_500;
		_body = get_error_page(_statusCode, _server, EP500);
    _contentLength = _body.size();
    
  }
}