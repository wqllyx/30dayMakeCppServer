#pragma once

class InetAddress;

class Socket
{
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind1(InetAddress*);
    void listen1();
    int setnonblocking();

    int accept1(InetAddress*);

    int getFd();
};
