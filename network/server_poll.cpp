

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
#include <poll.h>

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

    struct pollfd fds[1024] = {0};
    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
    int max_fd = sockfd;

    while(g_running.load()){
        int ret = poll(fds, max_fd + 1, -1);
        if(ret == -1){
            perror("poll");
            break;
        }
        if(fds[sockfd].revents & POLLIN){

            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
            if(client_fd == -1){
                perror("accept");
                continue;
            }
            fds[client_fd].fd = client_fd;
            fds[client_fd].events = POLLIN;
            max_fd = client_fd;
        }
        for(int i = sockfd + 1; i <= max_fd; i++){
            if(fds[i].revents & POLLIN){
                char buffer[1024];
                ssize_t read_len = recv(i, buffer, sizeof(buffer), 0);
                // printf("recv data: %s\n", buffer);
                std::string msg(buffer, buffer + read_len);
                if(msg == "quit"){
                    fds[i].fd = -1;
                    fds[i].events = 0;
                    close(i);
                    continue;
                }
                send(i, buffer, read_len, 0);
                printf("send data: %s\n", buffer);
            }
        }
    }

    for(int i = 0; i <= max_fd; i++){
        if(fds[i].fd != -1){
            fds[i].fd = -1;
            fds[i].events = 0;
            close(fds[i].fd);
        }
    }
    close(sockfd);
    return 0;
}