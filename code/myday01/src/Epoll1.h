#pragma once

#include<sys/epoll.h>
#include<vector>
#include "Channel.h"
#define MAX_EVENTS 1000
class Channel;


class Epoll1
{
private:
    int epfd;
    // epoll用于存放就绪事件（不是监听事件，监听事件存在内核中，使用红黑树存储，而不是数组）的数组。最多可监听MAX_EVENTS个就绪事件。
    struct epoll_event *events;
public:
    Epoll1();
    ~Epoll1();

    void addFd(int fd, uint32_t op);
    std::vector<Channel*> poll(int timeout = -1);
    void updateChannel(Channel*);
};


