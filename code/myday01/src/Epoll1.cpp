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
std::vector<Channel *> Epoll1::poll(int timeout){
    std::vector<Channel *> activeChannels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errorif(nfds == -1, "epoll wait: ");
    for(int i = 0; i < nfds; ++i){
        Channel *ch = (Channel*)events[i].data.ptr;
        ch->setRevents(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

/* 将channel对象添加到epoll监听列表 */
void Epoll1::updateChannel(Channel *channel){
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();

    if(!channel->getInEpoll()){
        errorif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        channel->setInEpoll();
        // debug("Epoll: add Channel to epoll tree success, the Channel's fd is: ", fd);
    } else{
        errorif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
        // debug("Epoll: modify Channel in epoll tree success, the Channel's fd is: ", fd);
    }
}
