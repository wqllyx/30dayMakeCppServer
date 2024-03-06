#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>  //这个头文件包含了<netinet/in.h>，不用再次包含了
#include<cstring>
#include<cstdio>
#include "util.h"
#include <unistd.h>
#include<iostream>
#include <sys/epoll.h>
#include <fcntl.h>
using std::cout,std::endl;
#define MAX_EVENTS 10
#define READ_BUFFER 1024
void setnonblocking(int fd){
    int old_option = fcntl(fd,F_GETFL);/*获取文件描述符旧的状态标志*/
    int new_option = old_option| O_NONBLOCK;/*设置非阻塞标志*/
    fcntl(fd, F_SETFL, new_option);
}

int main(){
    struct sockaddr_in serv_addr;//socket所在服务器的地址(由ip地址和端口号标识)
    struct epoll_event ev, events[MAX_EVENTS];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errorif(sockfd == -1, "socket 创建");
    
    
    bzero(&serv_addr, sizeof(serv_addr)); //    确定对象被使用前已先被初始化。如果不清空，使用gdb调试器查看addr内的变量，会是一些随机值，未来可能会导致意想不到的问题。
    serv_addr.sin_family = AF_INET; // 设置地址族
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");// 设置IP地址
    serv_addr.sin_port = htons(8888); // Functions to convert between host and network byte order.计算机网络的字节序都是小端

   /* 为什么定义的时候使用专用socket地址（sockaddr_in）而绑定的时候要转化为通用socket地址（sockaddr），
    以及转化IP地址和端口号为网络字节序的inet_addr和htons等函数及其必要性，在游双《Linux高性能服务器编程》第五章第一节：socket地址API中有详细讨论。 */
    errorif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "bind"); // 将socket所在主机得网络地址与文件描述符绑定。
    errorif(listen(sockfd, SOMAXCONN) == -1, "listen"); // 监听套接字端口。处理来自客户端的请求。


    int epollfd = epoll_create1(0);
    errorif(epollfd == -1, "epoll create:");
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    errorif(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1, "epoll_ctl:");
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));


    for(;;)
    {
        int nfds;
        errorif( (nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1)) == -1, "epoll_wait");
        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == sockfd) {
                int clnt_sockfd;
                errorif((clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len)) == -1, "accept:");
                cout <<"new client " << clnt_sockfd << "IP: " << inet_ntoa(clnt_addr.sin_addr) << "Port: " << ntohs(clnt_addr.sin_port) <<endl;
                setnonblocking(clnt_sockfd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clnt_sockfd;
                errorif(epoll_ctl(epollfd, EPOLL_CTL_ADD, clnt_sockfd,&ev), "");
            } 
            else if ((events[n].events & EPOLLIN)) {
                char buffer[READ_BUFFER];
                for(;;){
                    bzero(buffer,sizeof(buffer));
                    ssize_t bytes_read = recv(events[n].data.fd, buffer, sizeof(buffer), 0);
                    if (bytes_read > 0){
                        cout << "客户端socket " << events[n].data.fd << "发送信息：" << buffer << endl;
                        send(events[n].data.fd, buffer, bytes_read, 0);           //将相同的数据写回到客户端
                    }
                    else if(bytes_read == -1 && errno == EINTR){  //客户端正常中断、继续读取
                        cout << "continue reading" << endl;
                        continue;
                    }
                    else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){//非阻塞IO，这个条件表示数据全部读取完毕
                        cout <<"finish reading once, errno " << errno << endl;;
                        break;
                    }
                    else if (bytes_read == 0){
                        cout << "客户端socket " << events[n].data.fd << "关闭" << endl;
                        close(events[n].data.fd);
                        break;
                    }
                
                }
            }
            else{

            }
        }
    }
}
