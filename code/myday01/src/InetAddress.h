#pragma once
#include<arpa/inet.h>
#include "util.h"
#include<iostream>
using std::cout, std::endl;
/* 网络地址类 */
class InetAddress
{
private:
    struct sockaddr_in addr;
    socklen_t addr_len;

public:

    InetAddress();
    InetAddress(const char *p, uint16_t port);
    ~InetAddress() = default;

    void setInetAddr(sockaddr_in _addr, socklen_t _addr_len);
    sockaddr_in getAddr();
    socklen_t getAddr_len();
};
