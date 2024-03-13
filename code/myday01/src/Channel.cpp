#include "Channel.h"
#include "EventLoop.h"
Channel::Channel(EventLoop *loop, int fd): loop(loop), fd(fd), events(0), revent(0), inEpoll(false){}

Channel::~Channel(){}

void Channel::handleEvent(){
    callback();
}

void Channel::enableReading(){
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}
uint32_t Channel::getRevents(){
    return revent;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(){
    inEpoll = true;
}

// void Channel::setEvents(uint32_t _ev){
//     events = _ev;
// }

void Channel::setRevents(uint32_t _ev){
    revent = _ev;
}
void Channel::setCallback(std::function<void()> _cb){
    callback = _cb;
}