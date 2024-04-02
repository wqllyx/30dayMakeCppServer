#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include <functional>
#define READ_BUFFER 1024

Server::Server(EventLoop *loop) : loop(loop), acceptor(nullptr)
{
    // acceptor:服务器启动时创建，目前只有一个实例，负责：建立服务器服务socket地址（ip，port）、接受新的客户端连接。
    acceptor = new Acceptor(loop);
    // 设置Acceptor的回调函数用于：处理新连接。（NewConnectionCallback）
    std::function<void(Socket *)> cb = std::bind(&Server::handleNewConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server()
{
    delete acceptor;
}

void Server::handleNewConnection(Socket *sock)
{
    Connection *conn = new Connection(loop, sock);
    std::function<void(Socket *)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[sock->getFd()] = conn;
}

void Server::deleteConnection(Socket *sock)
{
    Connection *conn = connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}
