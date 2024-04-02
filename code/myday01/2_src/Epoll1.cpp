#include "Epoll1.hpp"
#include "util.hpp"
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

/* 将要管理的fd与其对应要监听的事件放入到epoll事件表中。op选项指定事件类型 */
void Epoll1::addFd(int fd, uint32_t op){
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    errorif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event: ");
}

/* 返回就绪的IO事件 */
std::vector<Channel *> Epoll1::poll(int timeout){
    std::vector<Channel *> activeChannels;
    // 调用epoll_wait，持续阻塞直到获得就绪IO事件。存入events事件数组中。
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errorif(nfds == -1, "epoll wait: ");
    // 设置每个就绪事件对应的通道信息。就绪事件的data数据的ptr指针指向的就是存储IO数据的channel类。
    for(int i = 0; i < nfds; ++i){
        Channel *ch = (Channel*)events[i].data.ptr;
        ch->setRevents(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

/* 将channel对象添加（或更新）到epoll监听列表 */
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
