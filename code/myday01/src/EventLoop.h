#pragma once
class Epoll1;
class Channel;

// 功能：相当于是一个事件池（因为里面封装了ep）。它具有的功能：向池子里添加事件（updateChannel）、返回就绪的事件给服务器（loop）。这两个功能都是依赖epoll实现的。
// 本质上EventLoop类就是对epoll的进一步封装。   
class EventLoop
{
private:
    Epoll1 *ep;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *);
};



