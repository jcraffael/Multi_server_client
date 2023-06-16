#include "include/Socket.h"
#include "include/InetAddress.h"
#include "include/util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

Socket::Socket() : fd(-1){
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
    serv_addr = new InetAddress("127.0.0.1", 8888);
}
Socket::Socket(int _fd) : fd(_fd){
    errif(fd == -1, "socket create error");
    serv_addr = new InetAddress();
}

Socket::~Socket(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

void Socket::bind(/*InetAddress *addr*/){
    errif(::bind(fd, (sockaddr*)&serv_addr->addr, serv_addr->addr_len) == -1, "socket bind error");
}

void Socket::listen(){
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::connect(){
    errif(::connect(fd, (sockaddr*)serv_addr, sizeof(*serv_addr)) == -1, "socket connect error");
}
void Socket::setnonblocking(){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(/*InetAddress *addr*/){
    int clnt_sockfd = ::accept(fd, (sockaddr*)&serv_addr->addr, &serv_addr->addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    return clnt_sockfd;
}

int Socket::getFd(){
    return fd;
}