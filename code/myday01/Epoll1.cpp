#include "Epoll1.h"
#include "util.h"
#include <unistd.h>
#include <cstring>

Epoll1::Epoll1() : epfd(-1), events(nullptr){
    epfd = epoll_create1(0);
    errorif(epfd == -1, "epoll_create1: ");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll1::~Epoll1(){
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete [] events;
}

/* op选项指定事件类型 */
void Epoll1::addFd(int fd, uint32_t op){
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    errorif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event: ");
}

/* 返回就绪事件vector */
std::vector<epoll_event> Epoll1::poll(int timeout){
    std::vector<epoll_event> activeEvents;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errorif(nfds == -1, "epoll wait: ");
    for(int i = 0; i < nfds; ++i){
        activeEvents.push_back(events[i]);
    }
    return activeEvents;
}
