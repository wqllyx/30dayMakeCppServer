#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <string.h>
#include <unistd.h>

#define READ_BUFFER 1024

Server::Server(EventLoop *loop){
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind1(serv_addr);
    serv_sock->listen1(); 
    serv_sock->setnonblocking();

    Channel *servChannel = new Channel(loop, serv_sock->getFd());
    std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);
    servChannel->setCallback(cb);
    servChannel->enableReading();
}

Server::~Server(){

}

void Server::handleReadEvent(int sockfd){
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

void Server::newConnection(Socket *serv_sock){
    InetAddress *clnt_addr = new InetAddress();  //会发生内存泄露！没有delete
    Socket *clnt_sockfd = new Socket(serv_sock->accept1(clnt_addr));  //会发生内存泄露！没有delete
    cout <<"new client " << clnt_sockfd->getFd() << "IP: " << inet_ntoa(clnt_addr->addr.sin_addr) << "Port: " << ntohs(clnt_addr->addr.sin_port) <<endl;
    clnt_sockfd->setnonblocking();
    Channel *clntChannel = new Channel(loop, clnt_sockfd->getFd());
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sockfd->getFd());
    clntChannel->setCallback(cb);
    clntChannel->enableReading();
}


