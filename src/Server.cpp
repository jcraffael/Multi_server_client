#include "Server.h"
#include "Socket.h"
#include "mes_process.hpp"
//include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <string.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>

#define READ_BUFFER 1024

Server::Server(EventLoop *_loop) : loop(_loop){    
    serv_sock = new Socket();
    //serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(/*serv_addr*/);
    serv_sock->listen(); 
    serv_sock->setnonblocking();
       
    servChannel = new Channel(loop, serv_sock->getFd());
    std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);
    servChannel->setCallback(cb);
    servChannel->enableReading();
}

Server::~Server()
{
    delete serv_sock;
    /*delete serv_addr;*/
    delete servChannel;
}

void Server::handleReadEvent(int sockfd){
    char buf[READ_BUFFER];
    while(true){    //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            mes_process m_process(buf, 1);
            rep_process r_process;
            r_process.set_code_val(m_process.get_act(), m_process.get_cont(), m_process.get_val());
            bzero(&buf, sizeof(buf));
            memcpy(buf, (char *)(r_process.get_message()), sizeof(buf));
            write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
            printf("continue reading");
            continue;
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(bytes_read == 0){  //EOF，客户端断开连接
            printf("EOF, client fd %d disconnected.\n", sockfd);
            
            delete connections[sockfd];
            connections.erase(sockfd);
            close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}

void Server::newConnection(Socket *serv_sock){
    //Socket *clnt_sock = new Socket(serv_sock->accept());       //会发生内存泄露！没有delete
    int new_fd = serv_sock->accept();
    printf("new client fd %d!\n"/*IP: %s Port: %d\n"*/, new_fd); //->getFd());, inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    //clnt_sock->setnonblocking();
    fcntl(new_fd, F_SETFL, fcntl(new_fd, F_GETFL) | O_NONBLOCK);
    Channel *clntChannel = new Channel(loop, new_fd); //clnt_sock->getFd());
    //printf("clnt channel addr is %p and fd addr is %p\n", (void *)clntChannel, (void *)&new_fd);
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, new_fd); //clnt_sock->getFd());
    clntChannel->setCallback(cb);
    clntChannel->enableReading(); // Add newly created channel(fd) to epoll events
    connections.insert(std::make_pair(new_fd, clntChannel));

}