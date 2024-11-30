/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseError.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:20:37 by ryanagit          #+#    #+#             */
/*   Updated: 2024/11/29 19:41:33 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"
#include"MethodUtils.hpp"

void HTTPResponse::makeBodyError(HTTPRequest& request)
{
    _statusCode = STATUS_400;
    _body = get_error_page(_statusCode, _server, EP400);
    _contentLength = _body.size();
}
