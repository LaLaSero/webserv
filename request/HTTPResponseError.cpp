/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseError.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yutakagi <yutakagi@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 19:20:37 by ryanagit          #+#    #+#             */
/*   Updated: 2024/12/15 01:31:06 by yutakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"
#include"MethodUtils.hpp"

void HTTPResponse::makeBodyError(HTTPRequest& request)
{
    _statusCode = STATUS_400;
    _body = get_error_page(_statusCode, _server, EP400);
    _contentLength = _body.size();
    (void)request;
}
