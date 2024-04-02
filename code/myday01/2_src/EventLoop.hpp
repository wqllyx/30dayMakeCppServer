#pragma once
class Epoll1;
class Channel;

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



