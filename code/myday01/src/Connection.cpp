#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
using namespace std;
#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop, Socket *_sock) : loop(_loop), sock(_sock), channel(nullptr)
{
    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setCallback(cb);
    channel->enableReading();
}

Connection::~Connection()
{
    delete channel;
    delete sock;
}

void Connection::echo(int sockfd)
{
    char buffer[READ_BUFFER];
    for (;;)
    {
        bzero(buffer, sizeof(buffer));
        ssize_t bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytes_read > 0)
        {
            cout << "客户端socket " << sockfd << "发送信息：" << buffer << endl;
            send(sockfd, buffer, bytes_read, 0); // 将相同的数据写回到客户端
        }
        else if (bytes_read == -1 && errno == EINTR)
        { // 客户端正常中断、继续读取
            cout << "continue reading" << endl;
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        { // 非阻塞IO，这个条件表示数据全部读取完毕
            cout << "finish reading once, errno " << errno << endl;
            ;
            break;
        }
        else if (bytes_read == 0)
        {
            cout << "客户端socket " << sockfd << "关闭" << endl;
            close(sockfd);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _cb)
{
    deleteConnectionCallback = _cb;
}