#include "Acceptor.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"
#include<iostream>
using namespace std;

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop)
{
    sock = new Socket();
    addr = new InetAddress("127.0.0.1", 8888);
    sock->bind1(addr);
    sock->listen1(); 
    sock->setnonblocking();
    // 接受通道字段：存储被接受的连接的数据信息（连接套接字fd，本通道请求事件类型events，epoll上就绪的事件类型revent）。
    acceptChannel = new Channel(loop, sock->getFd());
    // 设置acceptChannel的回调函数用于：接受连接。  这里cb绑定到acceptConnection函数上，传递给channel，当channel调用它的处理实践时就会调用Acceptor的acceptConnection函数
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
}

Acceptor::~Acceptor(){
    delete sock;
    delete addr;
    delete acceptChannel;
}

void Acceptor::acceptConnection(){
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(sock->accept1(clnt_addr));
    cout << "new client fd: " << clnt_sock->getFd() << "ip: " << inet_ntoa(clnt_addr->getAddr().sin_addr) << "port: " << ntohs(clnt_addr->getAddr().sin_port)<<endl;
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> call_back){
    newConnectionCallback = call_back;
}