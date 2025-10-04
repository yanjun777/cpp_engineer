
//  最好每次重置一下buffer 否则会很奇怪
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
#include <sys/stat.h>
#include <fcntl.h>
// 循环控制
static std::atomic<bool> g_running{true};
// 缓冲区大小
const int BUFFER_SIZE = 9192;
// 最大连接数
const int MAX_CONN = 1024;
typedef int (*Callback)(int);
// epoll文件描述符
int epoll_fd;

#define ENABLE_HTTP_REQUEST 1

void handle_sigint(int){
    g_running.store(false);
} 

struct connItem{
    int fd;
    char buffer[BUFFER_SIZE]; //指针 
    int index;
    char recv_buffer[BUFFER_SIZE];
    int recv_index;
    char send_buffer[BUFFER_SIZE];
    int send_index;
    union{
        Callback recv_cb;
        Callback listen_cb;
    }recv_t;
    char resource[BUFFER_SIZE]; 
    Callback send_cb;
};
// 连接项数组 
connItem conn_items[MAX_CONN];

#if ENABLE_HTTP_REQUEST
    typedef struct connItem connection_t;
    int http_response(connection_t *conn){
    #if 1
        int file_fd = open("index.html", O_RDONLY);
        if(file_fd == -1){
            perror("open");
            return -1;
        }
        struct stat file_stat;
        fstat(file_fd, &file_stat);

        conn->send_index = sprintf(conn->send_buffer,
             "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\n\r\n", 
             file_stat.st_size);

        ssize_t read_len = read(file_fd, conn->send_buffer+conn->send_index, BUFFER_SIZE-conn->send_index);
        if(read_len == -1){
            perror("read");
            return -1;
        }
        conn->send_index += read_len; 
        close(file_fd);
        printf("responing request\n"); 
        return conn->send_index;
    #else
        conn->send_index = sprintf(conn->send_buffer,
            "HTTP/1.1 200 OK\r\n"
            "Accept-Ranges: bytes\r\n"
            "Content-Length: 82\r\n"
            "Content-Type: text/html\r\n"
            "Date: Sat, 06 Aug 2023 13:16:46 GMT\r\n\r\n"
            "<html><head><title>voice.king</title></head><body><h1>King</h1></body></html>\r\n\r\n\r\n");
        return conn->send_index;
    #endif
    }
    int http_request(connection_t *conn){
        return 0; 
    }
#endif

void set_event(int fd, int event,int flag);
int send_callback(int fd);
int recv_callback(int fd);
int listen_callback(int fd){
    // 接受新连接
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
    if(client_fd == -1){
        perror("accept");
        return -1;
    }
    // 注册新连接
    set_event(client_fd, EPOLLIN, 1);
    // fd哈希表更新
    conn_items[client_fd].recv_t.recv_cb = recv_callback;
    conn_items[client_fd].send_cb = send_callback; 
    return client_fd;
}
int recv_callback(int fd){
    // 接受buffer 
    // 打印
    // 设置事件状态
    size_t recv_index = conn_items[fd].recv_index;
    char * recv_buffer = conn_items[fd].recv_buffer;
    ssize_t read_len = recv(fd, recv_buffer+recv_index,BUFFER_SIZE-recv_index, 0);
    if(read_len == 0){
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
    }
    
    // 打印消息
    std::string msg(recv_buffer+recv_index, conn_items[fd].recv_index);
    while (!msg.empty() && (msg.back() == '\n' || msg.back() == '\r' || msg.back() == '\0')) {
        msg.pop_back();
    }
    std::cout<<"recv msg:"<<msg<<std::endl; 
    // 判断是否quit 
    if(msg == "quit"){
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        close(fd);
    }
    // 更新index
    conn_items[fd].recv_index += read_len;
    std::cout<<"recv len:"<<read_len<<std::endl;
    // echo or response 
#if ENABLE_HTTP_REQUEST
    // http resquest handle 
    // http_request(&conn_items[fd]);
    http_response(&conn_items[fd]);
#else
    memcpy(conn_items[fd].send_buffer,conn_items[fd].recv_buffer+recv_index,read_len);
    conn_items[fd].send_index = read_len; 
#endif
    set_event(fd,EPOLLOUT,0); 

    
    return read_len;
}
int send_callback(int fd){
    // 发送buffer 
    // 设置事件状态 
    // printf("fd %d sending msg",fd); 
    std::cout<<"fd "<<fd<<" sending msg:"<<conn_items[fd].send_buffer;
    char *buffer = conn_items[fd].send_buffer;
    ssize_t send_len = send(fd, buffer, conn_items[fd].send_index, 0);
    if(send_len == -1){
        perror("send");
        return -1;
    }
    memset(buffer,0,send_len);// send_len or send_index 
    set_event(fd, EPOLLIN, 0);
    return send_len;
}

void set_event(int fd, int event,int flag){
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if(flag == 1){
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    }else{
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
    }
}

int main() {
    // 安装信号处理器
    std::signal(SIGINT, handle_sigint);
    try{
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
        
        printf("socket listening "); 
        // 创建epoll实例 返回epoll_fd
        epoll_fd = epoll_create(1);
        if(epoll_fd == -1){
            perror("epoll_create1");
            return -1;
        }
        set_event(sockfd, EPOLLIN, 1);
        conn_items[sockfd].recv_t.listen_cb = listen_callback; 

        // 事件数组 
        struct epoll_event events[1024];
        while(g_running.load()){
            
            int nready = epoll_wait(epoll_fd, events, 1024, -1);
            if(nready == -1){
                perror("epoll_wait");
                break;
            }
            for(int i = 0; i < nready; i++){
                int connfd = events[i].data.fd;
                // 事件读取 
                // printf("reading event\n");
                if(events[i].events & EPOLLIN ){
                    if(connfd == sockfd){
                        int clientfd = conn_items[connfd].recv_t.listen_cb(connfd);
                        printf("new client connected: %d\n", clientfd); 
                    }else{
                        int recv_l = conn_items[connfd].recv_t.recv_cb(connfd);
                    }
                }else if(events[i].events & EPOLLOUT){ 
                    int send_l = conn_items[connfd].send_cb(connfd);
                }
            }
        }
        // 释放所有资源
        for(int i = 0; i < 1024; i++){
            if (conn_items[i].fd >= 0) {
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, conn_items[i].fd, nullptr);
                close(conn_items[i].fd);
                conn_items[i].fd = -1;
                conn_items[i].index = 0;
            }
        } 
        close(epoll_fd);
        close(sockfd);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }

    
    return 0;
}