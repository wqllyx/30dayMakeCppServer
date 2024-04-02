#include "src/EventLoop.h"
#include "src/Server.h"

int main() {
    /* 
    1.创建事件循环对象：内部会创建epoll对象用于后续的IO事件复用（代理）。 */
    EventLoop *loop = new EventLoop();  // 封装事件循环对象：调用成员函数loop循环监听服务器上是否有事件发生，将其注册到epoll数据成员的内核监听队列中。
    Server *server = new Server(loop);  // 建立服务器对象：建立监听套接字（通道），并将监听套接字注册到epoll内核就绪事件表（红黑树）中，等待监听套接字上有事件（这里基本上就是负责来自客户端的连接事件）发生。
    // 启动主循环
    loop->loop();  
    return 0;
}