#include "Socket.h"
#include "util.h"
#include "InetAddress.h"
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include<cstring>
Socket::Socket() : fd(-1){
    errorif( (fd = socket(AF_INET, SOCK_STREAM, 0)) ==-1, "socket: ");
}

Socket::Socket(int _fd) : fd(_fd){
    errorif(fd==-1, "socket: ");
}

Socket::~Socket(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

void Socket::bind1(InetAddress *_addr){
    struct sockaddr_in addr = _addr->getAddr();
    socklen_t addr_len = _addr->getAddr_len();
    errorif(::bind(fd, (sockaddr *)&addr, addr_len) == -1, "socket bind error");
    _addr->setInetAddr(addr, addr_len);
}

void Socket::listen1(){
    errorif(::listen(fd, SOMAXCONN) == -1, "listen1: ");
}

/* 设置此socket为非阻塞 */
int Socket::setnonblocking(){
    int old_option = fcntl(fd,F_GETFL);     /*获取文件描述符旧的状态标志*/
    int new_option = old_option| O_NONBLOCK;/*设置非阻塞标志*/
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

/* 返回客户端socket */
int Socket::accept1(InetAddress *_addr){
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    bzero(&addr, sizeof(addr));
    int clnt_sockfd = ::accept(fd, (sockaddr *)&addr, &addr_len);
    errorif(clnt_sockfd == -1, "socket accept error");
    _addr->setInetAddr(addr, addr_len);
    return clnt_sockfd;
}

int Socket::getFd(){
    return fd;
}
