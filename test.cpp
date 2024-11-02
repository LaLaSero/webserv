#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_EVENTS 10

void start_server() {
    int listen_fd, conn_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // ソケットの作成
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        throw std::runtime_error("Socket creation failed");
    }

    // ソケットオプションの設定
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // サーバーアドレスの設定
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // すべてのインターフェースで待ち受け
    server_addr.sin_port = htons(PORT);

    // ソケットのバインド
    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        throw std::runtime_error("Bind failed");
    }

    // リスニングの開始
    if (listen(listen_fd, 5) == -1) {
        throw std::runtime_error("Listen failed");
    }

    // epollのセットアップ
    int epfd = epoll_create1(0);
    if (epfd == -1) {
        throw std::runtime_error("epoll_create failed");
    }

    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN; // 読み込みイベントを監視
    ev.data.fd = listen_fd;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) == -1) {
        throw std::runtime_error("epoll_ctl failed");
    }

    while (true) {
        // イベントの待機
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == listen_fd) {
                // 新しい接続を受け入れ
                conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addr_len);
                if (conn_fd == -1) {
                    std::cerr << "Accept failed" << std::endl;
                    continue;
                }
                std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) << std::endl;

                // 新しいソケットをepollに追加
                ev.events = EPOLLIN;
                ev.data.fd = conn_fd;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev) == -1) {
                    throw std::runtime_error("epoll_ctl failed");
                }
            } else {
                // データの読み込み
                char buffer[1024];
                int bytes_read = read(events[i].data.fd, buffer, sizeof(buffer));
                if (bytes_read <= 0) {
                    // 接続が閉じられた場合
                    std::cout << "Closing connection" << std::endl;
                    close(events[i].data.fd);
                } else {
                    // 読み込んだデータを表示
                    std::cout << "Received: " << std::string(buffer, bytes_read) << std::endl;
                }
            }
        }
    }

    // クリーンアップ
    close(listen_fd);
    close(epfd);
}

int main() {
    try {
        start_server();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
