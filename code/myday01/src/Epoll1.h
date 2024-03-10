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
    struct epoll_event *events;
public:
    Epoll1();
    ~Epoll1();

    void addFd(int fd, uint32_t op);
    std::vector<Channel*> poll(int timeout = -1);
    void updateChannel(Channel*);
};


