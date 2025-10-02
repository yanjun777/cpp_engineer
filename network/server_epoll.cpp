

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
#include <sys/epoll.h>

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
    // 创建epoll实例 返回epoll_fd
    int epoll_fd = epoll_create1(EPOLL_CLOEXEC);
    if(epoll_fd == -1){
        perror("epoll_create1");
        return -1;
    }
    // epoll_event用于存储事件 注册到epoll的文件描述符
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = sockfd;
    // 注册sockfd到epoll_fd
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event);

    struct epoll_event events[1024];
    while(g_running.load()){
        
        int nready = epoll_wait(epoll_fd, events, 1024, -1);
        if(nready == -1){
            perror("epoll_wait");
            break;
        }
        for(int i = 0; i < nready; i++){
            int connfd = events[i].data.fd;
            if(connfd == sockfd){
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
                if(client_fd == -1){
                    perror("accept");
                    continue;
                }
                event.events = EPOLLIN;
                event.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
                printf("new client connected: %s\n", inet_ntoa(client_addr.sin_addr)); 
            }else if(events[i].events & EPOLLIN){
                char buffer[1024];
                ssize_t read_len = recv(connfd, buffer, sizeof(buffer), 0);
                if(read_len == 0){
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, connfd, &event);
                    close(connfd);
                }
                std::string msg(buffer, buffer + read_len);
                while (!msg.empty() && (msg.back() == '\n' || msg.back() == '\r' || msg.back() == '\0')) {
                    msg.pop_back();
                }
                std::cout<<"recv msg:"<<msg<<std::endl; 
                if(msg == "quit"){
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, connfd, &event);
                    close(connfd);
                }
            }
        }
    }
    // 释放所有资源
    for(int i = 0; i < 1024; i++){
        if(events[i].data.fd != -1){
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &event);
            close(events[i].data.fd);
        }
    } 
    close(epoll_fd);
    close(sockfd);

    
    return 0;
}