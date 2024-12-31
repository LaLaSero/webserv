#pragma once
#include"Socket.hpp"
#include"SocketAddress.hpp"

class ClientSocket : public Socket {
 public:
     const std::string GetResponse(void) const;
     void SetResponse(std::string res);
     ClientSocket(int client_fd, SocketAddress client_address, const Config &conf);
     ClientSocket(int fd, const SocketAddress &server_addr,
                       const SocketAddress &client_addr,
                       const Config &config, int server_fd) ;
      int get_server_fd()const;
     bool IsShutdown();
     void SetIsShutdown(bool is_shutdown);
    void AppendRecvBuffer(const char* data, size_t len) {
        recv_buffer_.append(data, len);
    }
    std::string &GetRecvBuffer() {
        return recv_buffer_;
    }
 private:
     std::string recv_buffer_;
     const SocketAddress client_addr_;
     std::string response_;
     bool is_shutdown_;
     int server_fd_;
     ClientSocket();
     ClientSocket &operator=(const ClientSocket &rhs);

};
