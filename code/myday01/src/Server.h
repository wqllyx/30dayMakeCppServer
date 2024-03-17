#pragma once
#include<map>
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server
{
private:
    EventLoop *loop;
    Acceptor *acceptor; // 服务类，用于接受客户端连接
    std::map<int, Connection *> connections; // 连接类，所有连接到此服务器的客户端TCP。
public:
    Server(EventLoop*);
    ~Server();

    void handleNewConnection(Socket *serv_sock);
    void deleteConnection(Socket *sock);
};