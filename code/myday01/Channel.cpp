#include "Channel.h"
#include "Epoll1.h"
Channel::Channel(Epoll1 *ep, int fd): ep(ep), fd(fd), events(0), revent(0), inEpoll(false){}

Channel::~Channel(){}

void Channel::enableReading(){
    events = EPOLLIN | EPOLLET;
    
    ep->updateChannel(this);
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
