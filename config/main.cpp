/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:07:54 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/10/27 19:40:57 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
#include"Config.hpp"
#include"Parser.hpp"
#include"../server/EpollAdm.hpp"
#include"../server/SocketAddress.hpp"
#include"../server/ListenSocket.hpp"
#include"../server/ConnSocket.hpp"

#define DEFAULT_PATH "test.conf"

int InetPassiveSocket(const char *host, const char *service, int type,
                             SocketAddress *sockaddr, bool doListen,
                             int backlog) 
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, optval, s;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  hints.ai_socktype = type;
  hints.ai_family = AF_UNSPEC; /* Allow IPv4 or IPv6 */
  hints.ai_flags = AI_PASSIVE; /* Use wildcadrd IP address */

  s = getaddrinfo(host, service, &hints, &result);
  if (s != 0)
    return -1;

  /* Walk through returned list until we find an address structure
  that can be used to successfully create and bind a socket */
  optval = 1;
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype | SOCK_CLOEXEC | SOCK_NONBLOCK,
                 rp->ai_protocol);
    if (sfd == -1)
      continue; /* On error, try next address */

    if (doListen) {
      if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==
          -1) {
        close(sfd);
        freeaddrinfo(result);
        return -1;
      }
    }

    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break; /* Success */

    /* bind() failed: close this socket and try next address */
    close(sfd);
  }

  if (rp != NULL && doListen) {
    if (listen(sfd, backlog) == -1) 
	{
      freeaddrinfo(result);
      return -1;
    }
  }
  if (rp != NULL && sockaddr != NULL) 
  {
	sockaddr->set_socad(rp->ai_addr);
	sockaddr->set_length(rp->ai_addrlen);
  }
  freeaddrinfo(result);
  if (rp == NULL)
    return (-1);
  else
    return (sfd);
}


int InetListen(const std::string &host, const std::string &service, int backlog, SocketAddress *sockaddr) 
{
	const char *host_cstr;
  if (host.empty())
    host_cstr = NULL;
  else
    host_cstr = host.c_str();
  int fd = InetPassiveSocket(host_cstr, service.c_str(), SOCK_STREAM, sockaddr, true, backlog);
	if (fd < 0) 
    	throw std::runtime_error("InetListen Error");
	return fd;
}


FdEvent *CreateFdEvent(int fd, FdFunc func, void *data) {
  FdEvent *fde = new FdEvent();
  fde->fd = fd;
  fde->func = func;
  fde->timeout_ms = 0;
  fde->data = data;
  fde->state = 0;
  return fde;
}

void HandleListenSocketEvent(FdEvent *fde, unsigned int events, void *data, EpollAdm *epoll) 
{

}


void set_up_server(EpollAdm &epoll, Config &conf)
{
    // 使用済みのIP:ポートのリストを保持
    std::vector<std::string> used_ip_ports;
    // 開かれたファイルディスクリプタ（ソケット）のリストを保持
    std::vector<int> opened_fd;

    // Configから仮想サーバー（子サーバー）設定を取得
    std::vector<ChildServer> Children = conf.getchildserver();
    // 子サーバーごとに処理
    for (std::vector<ChildServer>::iterator it = Children.begin(); it != Children.end(); ++it)
    {
        // IPアドレスとポートを連結した文字列を作成
        std::string ip_port = it->get_listen_ip_() + ":" + it->get_listen_port_();
        std::string port_any_ip = "0.0.0.0:" + it->get_listen_port_();
        if (std::find(used_ip_ports.begin(), used_ip_ports.end(), ip_port) != used_ip_ports.end() ||
            std::find(used_ip_ports.begin(), used_ip_ports.end(), port_any_ip) != used_ip_ports.end())
        {
            continue;
        }
        // // ソケットアドレスを作成
        SocketAddress socket_address;
        // // ソケットを作成し、リッスン状態にする
       	int fd = InetListen(it->get_listen_ip_(), it->get_listen_port_(), SOMAXCONN, &socket_address);

        opened_fd.push_back(fd);  // 使われたFDをリストに追加

        // // // ソケットを`EpollAdm`に登録
        ListenSocket *listen_sock = new ListenSocket(fd, socket_address, conf);
        FdEvent *fde = CreateFdEvent(fd, HandleListenSocketEvent, listen_sock);
        epoll.register_event(fde);
        epoll.Add(fde, kFdeRead);  // 読み込みイベントを監視対象にする

        // // // 使用済みのIPとポートの組み合わせを記録
        used_ip_ports.push_back(ip_port);
    }
}


int main(int argc, char *argv[])
{
	std::string config_file_path = DEFAULT_PATH;
	if (2 < argc)
	{
		std::cerr << "Error:argv" << std::endl;
		return (1);
	}
	if (argc == 2)
		config_file_path = argv[1];
	try
	{
		Parser pas;
		pas.LoadFile(config_file_path);
		Config conf = pas.MakeConfig();

		EpollAdm epo;
		set_up_server(epo, conf);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Parser Error in Config file" << std::endl;
		std::cerr << e.what() << std::endl;
		std::exit(1);
	}
	
}