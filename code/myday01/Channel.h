#pragma once 
#include "Epoll1.h"
class Epoll1;
class Channel
{
private:
    /* data */
    Epoll1 *ep;
    int fd;
    uint32_t events;    //此文件描述符要监听哪些事件
    uint32_t revent;    //真实发生了什么事件
    bool inEpoll;
public:
    Channel(Epoll1*, int);
    ~Channel();

    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();

    void setRevents(uint32_t);
};


