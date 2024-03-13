#include "src/EventLoop.h"
#include "src/Server.h"

int main() {
    EventLoop *loop = new EventLoop();  // 封装事件循环对象：调用成员函数loop循环监听服务器上是否有事件发生，将其注册到epoll数据成员的内核监听队列中。
    Server *server = new Server(loop);  // 建立服务器对象：建立监听套接字，并将监听套接字注册到epoll内核就绪事件表中，等待监听事件发生。
    loop->loop();   // 循环监听事件发生，根据发生的事件类型，执行不同的处理逻辑。
    return 0;
}