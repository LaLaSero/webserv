/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:07:54 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/11/02 21:13:50 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
#include"Config.hpp"
#include"Parser.hpp"
#include"../server/EpollAdm.hpp"
#include"../server/SocketAddress.hpp"
#include"../server/ListenSocket.hpp"
#include"../server/ClientSocket.hpp"

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

void HandleClientSocketEvent(FdEvent *fde, unsigned int events, void *data, EpollAdm *epoll) 
{
    ClientSocket *client_sock = reinterpret_cast<ClientSocket *>(data);

    std::cout << "Start ClientSocket Event called" << std::endl;
    // 読み込みイベントの処理
    if (events & kFdeRead) {
        char buffer[1024];
        ssize_t nread = read(fde->fd, buffer, sizeof(buffer));
        if (nread == -1) {
            perror("read failed");
            close(fde->fd);
            epoll->delete_event(fde);
            delete fde;
            delete client_sock;
            return;
        }

        // クライアントが接続を切断した場合
        if (nread == 0) 
        {
            close(fde->fd);
            epoll->delete_event(fde);
            delete fde;
            delete client_sock;
            return;
        }

        
        buffer[nread] = '\0'; // null終端を追加
        std::string request(buffer); // 読み込んだデータを文字列に変換

        // リクエストを表示
        std::cout << "Request received:\n" << request << std::endl;
        // 読み込んだデータを処理する（例: HTTPリクエスト解析）
        // ここでリクエストに応じたレスポンスを作成
        std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World";

        // 書き込みイベントを登録する
        // client_sock->SetResponse(response);  // クライアントソケットにレスポンスを保存
        epoll->Modify(fde, kFdeWrite);       // 書き込み準備ができたら書き込みイベントを監視
    }

    // 書き込みイベントの処理
    if (events & kFdeWrite) {
        const std::string &response = client_sock->GetResponse();
        ssize_t nwritten = write(fde->fd, response.c_str(), response.size());
        if (nwritten == -1) {
            perror("write failed");
            close(fde->fd);
            epoll->delete_event(fde);
            delete fde;
            delete client_sock;
            return;
        }

        // 書き込み完了後に接続をクローズ
        std::cout << "Response sent, closing connection, fd: " << fde->fd << std::endl;
        close(fde->fd);
        epoll->delete_event(fde);
        delete fde;
        delete client_sock;
    }

    // エラーイベントの処理
    if (events & kFdeError) {
        std::cerr << "Error on client socket, fd: " << fde->fd << std::endl;
        close(fde->fd);
        epoll->delete_event(fde);
        delete fde;
        delete client_sock;
    }
}


void HandleListenSocketEvent(FdEvent *fde, unsigned int events, void *data, EpollAdm *epoll) 
{
    // std::cout <<"HandleListen called" << std::endl;
    // ListenSocketを取得
    (void)fde;
    ListenSocket *listen_sock = reinterpret_cast<ListenSocket *>(data);
    if (events & kFdeRead)
    { 
      // 新しいクライアント接続を受け入れる
      struct sockaddr_storage client_addr;
      socklen_t addrlen = sizeof(struct sockaddr_storage);
      ClientSocket *result = listen_sock->AcceptNewConnection();
      FdEvent *client_fde = CreateFdEvent(result->GetFd(),  HandleClientSocketEvent, result);
      // epollに新しいクライアント接続を監視対象として登録
      epoll->register_event(client_fde);
      epoll->Add(client_fde, kFdeRead); // 読み込みイベントを監視
    }
  if (events & kFdeError) 
  {
    throw std::runtime_error("Errorrrrrr");
  }
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
            continue;
        // // ソケットアドレスを作成
        SocketAddress socket_address;
        // // ソケットを作成し、リッスン状態にする
       	int fd = InetListen(it->get_listen_ip_(), it->get_listen_port_(), SOMAXCONN, &socket_address);
        if(fd < 0)
          throw std::runtime_error("InetListen Error");
        // // // ソケットを`EpollAdm`に登録
        ListenSocket *listen_sock = new ListenSocket(fd, socket_address, conf);
        FdEvent *fde = CreateFdEvent(fd, HandleListenSocketEvent, listen_sock);
        epoll.register_event(fde);
        epoll.Add(fde, kFdeRead);  // 読み込みイベントを監視対象にする
        // // // 使用済みのIPとポートの組み合わせを記録
        used_ip_ports.push_back(ip_port);
        opened_fd.push_back(fd); 
    }
}

void InvokeFdEvent(FdEvent *fde, unsigned int events, EpollAdm *epoll) 
{
  fde->func(fde, events, fde->data, epoll);
}

void Loop(EpollAdm &epoll) {
  std::cout << "Start Loop" << std::endl;
  while (1) 
  {
    std::vector<FdandEvent> timeouts = epoll.RetrieveTimeouts();
    for (std::vector<FdandEvent>::const_iterator it = timeouts.begin();it != timeouts.end(); ++it) 
    {
      FdEvent *fde = it->fde;
      unsigned int events = it->events;
      InvokeFdEvent(fde, events, &epoll);
    }
    std::vector<FdandEvent> result = epoll.WaitEvents(100);
    for (std::vector<FdandEvent>::const_iterator it = result.begin();it != result.end(); ++it) 
    {
      FdEvent *fde = it->fde;
      unsigned int events = it->events;
      // std::cout << "Event received for fd: " << fde->fd << ", events: " << events << std::endl;
      InvokeFdEvent(fde, events, &epoll);
    }
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
    Loop(epo);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::exit(1);
	}
}