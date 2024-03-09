#include "util.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Epoll1.h"
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h> 
#include<cstring>
#include <unistd.h>
#include<iostream>
#include <sys/epoll.h>
// #include <fcntl.h>
using std::cout,std::endl;
#define READ_BUFFER 1024

void handleReadEvent(int sockfd);

int main(){

    // 1. 创建Socket
    Socket *serv_sock = new Socket;
    // 2. 分配socket地址，命名socket
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind1(serv_addr);
    // 3. 监听socket
    serv_sock->listen1(); 
    // 4. 创建并添加监听socket到epoll事件表。
    Epoll1 *ep = new Epoll1;   
    serv_sock->setnonblocking(); // 设置非阻塞
    Channel *servChannel = new Channel(ep, serv_sock->getFd());
    servChannel->enableReading();
    for(;;)
    {
        // 创建channel表
        std::vector<Channel*> activeChannels = ep->poll(-1);
        int nfds = activeChannels.size();
        for (int n = 0; n < nfds; ++n) {
            // 获得channel关联的fd，判断此fd是服务器监听fd，还是客户端请求fd
            int chfd = activeChannels[n]->getFd();
            if (chfd == serv_sock->getFd()) {
                InetAddress *clnt_addr = new InetAddress();  //会发生内存泄露！没有delete
                Socket *clnt_sockfd = new Socket(serv_sock->accept1(clnt_addr));  //会发生内存泄露！没有delete
                cout <<"new client " << clnt_sockfd->getFd() << "IP: " << inet_ntoa(clnt_addr->addr.sin_addr) << "Port: " << ntohs(clnt_addr->addr.sin_port) <<endl;
                clnt_sockfd->setnonblocking();
                Channel *clntChannel = new Channel(ep, clnt_sockfd->getFd());
                clntChannel->enableReading();
            } 
            else if ((activeChannels[n]->getRevents() & EPOLLIN)) {
                handleReadEvent(activeChannels[n]->getFd());
            }
            else{
                cout << "else"<< endl;
            }
        }
    }
    delete serv_sock;
    delete serv_addr;
    return 0;
}

void handleReadEvent(int sockfd){
    char buffer[READ_BUFFER];
    for(;;){
        bzero(buffer,sizeof(buffer));
        ssize_t bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytes_read > 0){
            cout << "客户端socket " << sockfd << "发送信息：" << buffer << endl;
            send(sockfd, buffer, bytes_read, 0);           //将相同的数据写回到客户端
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
            cout << "客户端socket " << sockfd << "关闭" << endl;
            close(sockfd);
            break;
        }
    
    }
}
