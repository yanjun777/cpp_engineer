

#include <iostream>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <csignal>
#include <algorithm>

static std::atomic<bool> g_running{true};

void handle_sigint(int){
    g_running.store(false);
} 


int main() {
    // 安装信号处理器
    std::signal(SIGINT, handle_sigint);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { perror("socket"); return -1; }

    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) != 1) {
        perror("inet_pton");
        return -1;
    }

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return -1;
    }
    if (listen(sockfd, 10) == -1) { perror("listen"); return -1; }

    fd_set fds, read_fds;
    FD_ZERO(&fds);
    FD_SET(sockfd, &fds);

    while(g_running.load()){
        read_fds = fds;
        int ret = select(FD_SETSIZE, &read_fds, NULL, NULL, NULL);
        if(ret == -1){
            perror("select");
            break;
        }
        if(FD_ISSET(sockfd, &read_fds)){
            int client_fd = accept(sockfd, NULL, NULL);
            if(client_fd == -1){
                perror("accept");
                continue;
            }
            FD_SET(client_fd, &fds);
            std::cout<<"client connected"<<std::endl;
        }
        for(int i = sockfd + 1; i < FD_SETSIZE; i++){
            if(FD_ISSET(i, &read_fds)){
                char buffer[1024];
                ssize_t read_len = recv(i, buffer, sizeof(buffer), 0);
                // 使用收到的长度构造字符串，判断 quit
                std::string msg(buffer, buffer + read_len);
                // 去掉尾部换行/回车
                while (!msg.empty() && (msg.back() == '\n' || msg.back() == '\r' || msg.back() == '\0')) {
                    msg.pop_back();
                }
                if(msg == "quit"){
                    std::cout << "client requested close" << std::endl;
                    FD_CLR(i, &fds);
                    close(i);
                    break;
                }
                std::cout << "recv data: " << msg << std::endl;
                ssize_t sent = send(i, buffer, read_len, 0);
                if (sent < 0) {
                    perror("send");
                    break;
                }
                std::cout << "echoed: " << msg << std::endl;
            }

        }
    }
    return 0;
}