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
        // 返回监听到的IO就绪事件（Channel对象）：（事件相关数据（IO对象：fd，此对象要监听事件events（可以监听多个事件，以位图表示），此IO已经就绪的事件revent）
        chs = ep->poll();
        // 循环处理接收到的就绪事件。
        for(auto it = chs.begin(); it != chs.end(); ++it){
            // 此处会调用相应的回调函数：
            // 1.如果第一次走到这个循环，就只有一个fd上有事件发生（服务器上监听套接字的新客户端连接事件）：他的回调函数第一次绑定到std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
            // acceptChannel->setCallback(cb);在aceptconnection之中，会设置acceptor的回调函数为
            // 2.如果是第二次走到这个循环（）：那就按情况处理。
            // 在此函数中，会将handleevent绑定到IO读写函数handlereadevent（）
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *ch){
    ep->updateChannel(ch);
}