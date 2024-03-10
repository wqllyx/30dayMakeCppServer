#include "EventLoop.h"
#include "Epoll1.h"
#include "Channel.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false){
    ep = new Epoll1;
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
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *ch){
    ep->updateChannel(ch);
}