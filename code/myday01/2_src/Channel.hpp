#pragma once 
#include "sys/epoll.h"
#include <functional>
class EventLoop;
class Channel
{
private:
    /* data */
    EventLoop *loop;
    int fd;
    uint32_t events; // 此文件描述符要监听哪些事件
    uint32_t revent; // 真实发生了什么事件
    bool inEpoll;
    std::function<void()> callback;

public:
    Channel(EventLoop *, int);
    ~Channel();

    void handleEvent();
    void enableReading();

    int getFd();
    uint32_t getEvents();

    uint32_t getRevents();
    void setRevents(uint32_t);
    bool getInEpoll();
    void setInEpoll();
    
    void setCallback(std::function<void()>);
};
