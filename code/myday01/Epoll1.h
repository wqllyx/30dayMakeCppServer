#pragma once

#include<sys/epoll.h>
#include<vector>

#define MAX_EVENTS 1000

class Epoll1
{
private:
    int epfd;
    struct epoll_event *events;
public:
    Epoll1();
    ~Epoll1();

    void addFd(int fd, uint32_t op);
    std::vector<epoll_event> poll(int timeout = -1);
};


