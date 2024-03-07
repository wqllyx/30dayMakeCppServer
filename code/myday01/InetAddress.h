#pragma once
#include<arpa/inet.h>
#include "util.h"
#include<iostream>
using std::cout, std::endl;
/* 网络地址类 */
class InetAddress
{
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char *p, uint16_t port);
    ~InetAddress() = default;
};
