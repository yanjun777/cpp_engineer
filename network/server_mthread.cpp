

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
static int g_listen_fd = -1;
static std::mutex g_clients_mutex;
static std::vector<int> g_client_fds;

static void close_fd_safely(int fd) {
    if (fd >= 0) {
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }
}

static void handle_sigint(int) {
    g_running.store(false);
    // 关闭监听 FD 以打断 accept
    close_fd_safely(g_listen_fd);
    // 关闭所有客户端连接以让线程尽快退出
    std::lock_guard<std::mutex> lock(g_clients_mutex);
    for (int fd : g_client_fds) {
        close_fd_safely(fd);
    }
}

void handle_client(int client_fd){
    while(g_running.load()){
        char buffer[1024];
        ssize_t read_len = recv(client_fd, buffer, sizeof(buffer), 0);
        if (read_len == 0) {
            // 对端正常关闭
            break;
        }
        if(read_len < 0){
            if (errno == EINTR) {
                continue;
            }
            perror("recv");
            break;
        }
        // 使用收到的长度构造字符串，判断 quit
        std::string msg(buffer, buffer + read_len);
        // 去掉尾部换行/回车
        while (!msg.empty() && (msg.back() == '\n' || msg.back() == '\r' || msg.back() == '\0')) {
            msg.pop_back();
        }
        if(msg == "quit"){
            std::cout << "client requested close" << std::endl;
            break;
        }
        std::cout << "recv data: " << msg << std::endl;
        ssize_t sent = send(client_fd, buffer, read_len, 0);
        if (sent < 0) {
            perror("send");
            break;
        }
        std::cout << "echoed: " << msg << std::endl;
    }

    close_fd_safely(client_fd);
    // 从列表移除
    {
        std::lock_guard<std::mutex> lock(g_clients_mutex);
        auto it = std::remove(g_client_fds.begin(), g_client_fds.end(), client_fd);
        g_client_fds.erase(it, g_client_fds.end());
    }
}

int main() {
    // 安装信号处理器
    std::signal(SIGINT, handle_sigint);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { perror("socket"); return -1; }
    g_listen_fd = sockfd;

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
        close_fd_safely(sockfd);
        return -1;
    }
    if (listen(sockfd, 10) == -1) { perror("listen"); close_fd_safely(sockfd); return -1; }

    while(g_running.load()){
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == -1) {
            if (!g_running.load()) {
                break; // 关停中
            }
            if (errno == EINTR) {
                continue; // 被信号中断，继续检查运行状态
            }
            if (errno == EBADF || errno == EINVAL) {
                // 监听套接字已被关闭
                break;
            }
            perror("accept");
            break;
        }
        {
            std::lock_guard<std::mutex> lock(g_clients_mutex);
            g_client_fds.push_back(client_fd);
        }
        std::cout << "client connected" << std::endl;
        std::thread(handle_client, client_fd).detach(); 
    }

    // 退出前确保监听 FD 关闭
    close_fd_safely(sockfd);

    // 双保险：关闭残余客户端
    {
        std::lock_guard<std::mutex> lock(g_clients_mutex);
        for (int fd : g_client_fds) {
            close_fd_safely(fd);
        }
        g_client_fds.clear();
    }

    return 0;
}