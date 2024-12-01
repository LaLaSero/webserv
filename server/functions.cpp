/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryanagit <ryanagit@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:07:54 by yanagitaryu       #+#    #+#             */
/*   Updated: 2024/12/01 20:40:03 by ryanagit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
#include"../config/Config.hpp"
#include"../config/Parser.hpp"
#include"../server/EpollAdm.hpp"
#include"../server/SocketAddress.hpp"
#include"../server/ListenSocket.hpp"
#include"../server/ClientSocket.hpp"

#include"../request/HTTPResponse.hpp"
#include"../request/HTTPRequest.hpp"
#include"../request/ParseRequest.hpp"
#include"functions.hpp"

static void prepare_hints(struct addrinfo &hints, int type)
{
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  hints.ai_socktype = type;
  hints.ai_family = AF_UNSPEC;
  hints.ai_flags = AI_PASSIVE;
}


int PreparePassiveSocket(const char *host, const char *service, int type, SocketAddress *sockaddr, bool doListen,int backlog) 
{
  struct addrinfo hints;
  struct addrinfo *result;
  struct addrinfo *rp;
  int sfd;
  int res;


  // 名前空間の解決
  prepare_hints(hints, type);
  res = getaddrinfo(host, service, &hints, &result);
  if (res != 0)
    return (-1);
  int optval;
  optval = 1;
  for (rp = result; rp != NULL; rp = rp->ai_next) 
  {
    sfd = socket(rp->ai_family, rp->ai_socktype | SOCK_CLOEXEC | SOCK_NONBLOCK,
                 rp->ai_protocol);
    if (sfd == -1)
      continue; /* On error, try next address */
    if (doListen) 
    {
      if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==-1) 
      {
        close(sfd);
        freeaddrinfo(result);
        return (-1);
      }
    }
    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break; /* Success */
    /* bind() failed: close this socket and try next address */
    close(sfd);
  }
  if (rp != NULL && doListen) 
  {
    if (listen(sfd, backlog) == -1) 
	  {
      freeaddrinfo(result);
      return (-1);
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
  int fd = PreparePassiveSocket(host_cstr, service.c_str(), SOCK_STREAM, sockaddr, true, backlog);
	if (fd < 0)
    	throw std::runtime_error("InetListen Error");
	return fd;
}


FdEvent *CreateFdEvent(int fd, FdFunc func, void *data) 
{
  FdEvent *fde = new FdEvent();
  fde->fd = fd;
  fde->func = func;
  fde->timeout_ms = 0;
  fde->data = data;
  fde->state = 0;
  fde->original_clinet = NULL;
  return (fde);
}


// std::string make_parse_error_response(int i)
// {
//   if (i == errorno_notsup)
//   {
//     return ();
//   }
// }



bool Overread(ClientSocket *socket) {
    unsigned char buf[BUF_SIZE];
    int conn_fd = socket->GetFd();
    
    // データを読み取る
    int n = read(conn_fd, buf, sizeof(buf) - 1);
    
    // 読み込み失敗または接続切断時の処理
    if (n <= 0) 
    {  // EOF (TCP flag FIN) or Error
        socket->SetIsShutdown(true);
        return true;
    }
    // 読み取ったデータをクライアントソケットのバッファに追加
    return false;
}

bool isfinish()
{
  return (true);
}

void HandlePOSTSocketEvent(FdEvent *fde, unsigned int events, void *data, EpollAdm *epoll)
{
    bool should_close_client = true;
    std::cout << "POST writing starting" << std::endl;
    if (events & kFdeWrite) 
    {

        std::string *data_to_send = reinterpret_cast<std::string *>(fde->data);
        ssize_t nwritten = write(fde->fd, data_to_send->c_str(), data_to_send->size());
        if (nwritten == -1) 
        {
            perror("write failed");
            close(fde->fd);
            epoll->delete_event(fde);
            delete fde;
            return;
        }
        FdEvent *original_fde;
        original_fde = reinterpret_cast<FdEvent*>(fde->data);
        if (should_close_client)
        {
            fde->original_clinet->SetResponse("");
            epoll->GotoNextEvent(original_fde, kFdeWrite);
        }
        epoll->delete_event(fde);
        close(fde->fd);
    }
}

void HandleCgiSocketEvent(FdEvent *fde, unsigned int events, void *data, EpollAdm *epoll) 
{
    bool should_close_client = true;
    if (events & kFdeRead) 
    {
        // CGIの出力を読み取る
        std::cout << "start cgi reading" << std::endl;
        // `output_pipe` からデータを読み取り、クライアントに送信するなどの処理
        char buf[BUF_SIZE];
        ssize_t n = read(fde->fd, buf, sizeof(buf) - 1);
        buf[n] = '\0';  // Null-terminate the string
        if (n <= 0) 
        {  
            // EOF (TCP flag FIN) or Error
            should_close_client = true;
        }
        // 読み取ったデータを処理する（レスポンスをクライアントに送信など）
        std::cout << "CGI Response: " << buf << std::endl;
        // クライアントにデータを送る処理へ遷移（例: epollの書き込みイベントへ）
        FdEvent *original_fde;
        original_fde = reinterpret_cast<FdEvent*>(fde->data);
        if (should_close_client)
        {
            fde->original_clinet->SetResponse(buf);
            epoll->GotoNextEvent(original_fde, kFdeWrite);
        }
        epoll->delete_event(fde);
        close(fde->fd);
    }
    if (events & kFdeWrite) 
    {
        // CGIの入力にデータを書き込む
        std::cout << "start cgi writing" << std::endl;
        std::string data_to_send = "some data for cgi";
        ssize_t nwritten = write(fde->fd, data_to_send.c_str(), data_to_send.size());
        if (nwritten == -1) 
        {
            perror("write failed");
            close(fde->fd);
            epoll->delete_event(fde);
            delete fde;
            return;
        }
        std::cout << "cgi writing complete" << std::endl;
        epoll->delete_event(fde);
        close(fde->fd);
    }
    if (events & kFdeError) 
    {
        std::cerr << "Error on CGI socket" << std::endl;
        close(fde->fd);
        epoll->delete_event(fde);
        delete fde;
    }
}

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


void HandleClientSocketEvent(FdEvent *fde, unsigned int events, void *data, EpollAdm *epoll) 
{
    ClientSocket *client_sock = reinterpret_cast<ClientSocket *>(data);
    bool should_close_client = false;

    // 読み込みイベントの処理
    if (events & kFdeRead) 
    {
        std::cout << "start reading" << std::endl;
        char buf[BUF_SIZE];
        int conn_fd = client_sock->GetFd();
        ssize_t n = read(conn_fd, buf, sizeof(buf) - 1);
        buf[n] = '\0';  // Null-terminate the string
        std::cout << buf << std::endl;
        
        if (n <= 0) 
        {  
            // EOF (TCP flag FIN) or Error
            client_sock->SetIsShutdown(true);
            should_close_client = true;
        }

        // HTTPリクエストの解析
        HTTPRequest request;
        ParseRequest parser_request(request);
        parser_request.parse(buf);

        should_close_client = true;
        if (should_close_client || (request.getContentLength() > 0 && request.getBody().size() >= request.getContentLength()))
        {
            // HTTPレスポンスの準備
            HTTPResponse response(epoll->get_config());
            ChildServer server = epoll->get_config().FindServerfromFd(client_sock->get_server_fd());
            response.SetChildServer(&server);
            response.selectResponseMode(request);
            if (request.getMode() == POST_RESPONSE)
            {
              if (response.makeBodyPOST(request))
              {
                std::string res = response.makeBodyResponse();
                client_sock->SetResponse(res); // クライアントソケットにレスポンスを保存
                epoll->GotoNextEvent(fde, kFdeWrite);  // 書き込み準備ができたら書き込みイベントを監視
              }
              else
              {
                int fd = open(request.getUri().c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_ASYNC, 0644);
                if (fd == -1)
                {
                  response.HelpPostStatusChange(STATUS_500);
                  std::string res = response.makeBodyResponse();
                  client_sock->SetResponse(res); // クライアントソケットにレスポンスを保存
                  epoll->GotoNextEvent(fde, kFdeWrite);  // 書き込み準備ができたら書き込みイベントを監視
                }
                else
                {
                  std::string Body = request.getBody();
                  FdEvent *post_write_fde = CreateFdEvent(fd, HandlePOSTSocketEvent, &(Body));
                  post_write_fde->original_clinet = client_sock;
                  epoll->register_event(post_write_fde);
                  epoll->Add(post_write_fde, kFdeWrite);
                }
              } 
            }
            // CGIレスポンスが必要な場合
            if (request.getMode() == CGI_RESPONSE)
            {
                // CGIプロセス用の入力と出力のpipeを作成
                std::cout << "start cgi" << std::endl;
                int input_pipe[2], output_pipe[2];
                if (pipe(input_pipe) == -1 || pipe(output_pipe) == -1)
                {
                    perror("pipe failed");
                    return;
                }

                // CGIプロセスを実行
                pid_t pid = fork();
                if (pid == -1)
                {
                    perror("fork failed");
                    return;
                }
                else if (pid == 0)  // 子プロセス（CGI）
                {
                    // 子プロセスの標準入力を入力pipeに、標準出力を出力pipeにリダイレクト
                    dup2(input_pipe[0], STDIN_FILENO);  // input_pipe[0]を標準入力に接続
                    dup2(output_pipe[1], STDOUT_FILENO);  // output_pipe[1]を標準出力に接続

                    close(input_pipe[1]);
                    close(output_pipe[0]);

                    // 実際のCGIプログラムを実行（例: "/usr/bin/php"など）
                    char *args[] = { "python3", "/home/ryanagit/test.py", NULL };
                    execvp("python3", args);
                    perror("execl failed");
                    exit(1);
                }
                else  // 親プロセス（サーバー）
                {
                    // 親プロセス側でpipeの読み書きイベントをepollに登録
                    FdEvent *cgi_input_fde = CreateFdEvent(output_pipe[0], HandleCgiSocketEvent, fde);  // 入力pipeの読み込み
                    FdEvent *cgi_output_fde = CreateFdEvent(input_pipe[1], HandleCgiSocketEvent, NULL); // 出力pipeへの書き込み

                    std::cout << "input:" << input_pipe[0]<< std::endl;
                    std::cout << "output:" << output_pipe[1] << std::endl;
                    std::cout << "create cgi event" << std::endl;

                    cgi_input_fde->original_clinet = client_sock;
                    epoll->register_event(cgi_input_fde);
                    epoll->Add(cgi_input_fde, kFdeRead);  // 出力pipeの読み込みイベントを監視
                    epoll->register_event(cgi_output_fde);
                    epoll->Add(cgi_output_fde, kFdeWrite);  // 入力pipeへの書き込みイベントを監視
                }
            }
            else
            {
                // 通常のレスポンス処理
                std::string res = response.makeBodyResponse();
                client_sock->SetResponse(res); // クライアントソケットにレスポンスを保存
                epoll->GotoNextEvent(fde, kFdeWrite);  // 書き込み準備ができたら書き込みイベントを監視
            }
        }
    }

    // 書き込みイベントの処理
    if (events & kFdeWrite) 
    {
        std::cout << "start responsing" << std::endl;
        std::string response = client_sock->GetResponse();
        std::cout << response << std::endl;
        ssize_t nwritten = write(fde->fd, response.c_str(), response.size());
        if (nwritten == -1) 
        {
            perror("write failed");
            close(fde->fd);
            epoll->delete_event(fde);
            delete fde;
            delete client_sock;
            return;
        }
        // 書き込み完了後に接続をクローズ
        epoll->delete_event(fde);     
        delete client_sock;
    }

    if (events & kFdeError) 
    {
        std::cerr << "Error on client socket, fd: " << fde->fd << std::endl;
        close(fde->fd);
        epoll->delete_event(fde);
        delete fde;
        delete client_sock;
    }
}



void HandleListenSocketEvent(FdEvent *fde, unsigned int events, void *data, EpollAdm *epoll) 
{
  (void)fde;
  ListenSocket *listen_sock = reinterpret_cast<ListenSocket *>(data);
  if (events & kFdeRead)
  {
    ClientSocket *result = listen_sock->AcceptNewConnection();
    FdEvent *client_fde = CreateFdEvent(result->GetFd(),  HandleClientSocketEvent, result);
    epoll->register_event(client_fde);
    epoll->Add(client_fde, kFdeRead);
  }
  if (events & kFdeError)
    throw std::runtime_error("Errorrrrrr");
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
        if (it->get_listen_ip_().empty())
          it->set_listen_ip_("0.0.0.0");
        if (it->get_listen_port_().empty())
          it->set_listen_port_("8080");
        std::string ip_port = it->get_listen_port_();
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

        conf.AddFdandServers(fd, *it);
        std::cout << "fd:" << fd << std::endl;
    }
}

void AwakeFdEvent(FdEvent *fde, unsigned int events, EpollAdm *epoll) 
{
  fde->func(fde, events, fde->data, epoll);
}

void Loop(EpollAdm &epoll) 
{
  std::cout << "Start Loop" << std::endl;
  while (1) 
  {
    std::vector<FdandEvent> timeouts = epoll.RetrieveTimeouts();
    for (std::vector<FdandEvent>::const_iterator it = timeouts.begin();it != timeouts.end(); ++it) 
    {
      FdEvent *fde = it->fde;
      unsigned int events = it->events;
      AwakeFdEvent(fde, events, &epoll);
    }
    std::vector<FdandEvent> result = epoll.CheckEvents(100);
    for (std::vector<FdandEvent>::const_iterator it = result.begin();it != result.end(); ++it) 
    {
      FdEvent *fde = it->fde;
      unsigned int events = it->events;
      AwakeFdEvent(fde, events, &epoll);
    }
  }
}