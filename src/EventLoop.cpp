#include "include/EventLoop.h"
#include "include/Epoll.h"
#include "include/Channel.h"
#include <vector>
#include <cstdio>

EventLoop::EventLoop() : ep(nullptr), quit(false){
    ep = new Epoll();
}

EventLoop::~EventLoop()
{
    delete ep;
}
void EventLoop::loop(){
    while(!quit){
    std::vector<Channel*> chs;
        chs = ep->poll();
        for(auto it = chs.begin(); it != chs.end(); ++it){
            printf("Now handle event in eventloop:loop...\n");
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *ch){
    ep->updateChannel(ch);
}