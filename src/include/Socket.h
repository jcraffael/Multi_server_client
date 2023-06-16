#pragma once
#include <fcntl.h>
class InetAddress;
class Socket
{
private:
    int fd;
    InetAddress *serv_addr;
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(/*InetAddress*/);
    void listen();
    void setnonblocking();
    void connect();
    int accept(/*InetAddress*/);

    int getFd();
};

