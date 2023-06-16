#pragma once

#include <map>
class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Server
{
private:
    EventLoop *loop;
    Socket *serv_sock;
    Channel *servChannel;
    std::map<int, Channel*> connections;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
};

