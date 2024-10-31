/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnSocket.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:40:47 by ryanagit          #+#    #+#             */
/*   Updated: 2024/10/27 15:35:21 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include"Socket.hpp"
#include"SocketAddress.hpp"

class ConnSocket : public Socket {
//   const SocketAddress client_addr_;

//   std::deque<http::HttpRequest> requests_;
//   http::HttpResponse *response_;
//   utils::ByteVector buffer_;

//   // shutdown したかどうか
//   // サーバーから切る場合は shutdown() して FIN を送ったか
//   // クライアントから切る場合は FIN がサーバーに届いたか
//   bool is_shutdown_;

//  public:
//   // タイムアウトのデフォルト時間は5秒
//   // HTTP/1.1 では Connection: close が来るまでソケットを接続し続ける｡
//   // Nginx などでは5秒間クライアントからデータが来なければ
//   //   切断するのでそれに合わせる｡
//   static const long kDefaultTimeoutMs = 5 * 1000;

//   ConnSocket(int fd, const SocketAddress &server_addr,
//              const SocketAddress &client_addr, const config::Config &config);
//   virtual ~ConnSocket();

//   std::string GetRemoteIp() const;
//   std::string GetRemoteName() const;

//   std::deque<http::HttpRequest> &GetRequests();

//   bool HasParsedRequest();
//   void ShutDown();

//   http::HttpResponse *GetResponse();
//   void SetResponse(http::HttpResponse *response);
//   bool IsShutdown();
//   void SetIsShutdown(bool is_shutdown);

//   utils::ByteVector &GetBuffer();

 private:
  ConnSocket();
  ConnSocket &operator=(const ConnSocket &rhs);
};
