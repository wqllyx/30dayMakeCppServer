#pragma once
class InetAddress;
/* 网络IO接口类：
负责创建用于网络通信的socket，并提供网络地址绑定功能（命名socket），接受连接功能（服务器），监听功能（服务器） */
class Socket
{
private:
    int fd;

public:
    Socket();
    Socket(int);
    ~Socket();

    void bind1(InetAddress *);
    void listen1();
    int setnonblocking();

    int accept1(InetAddress *);

    int getFd() const;
};
