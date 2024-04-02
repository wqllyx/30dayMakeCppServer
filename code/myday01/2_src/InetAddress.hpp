#pragma once
#include<arpa/inet.h>
#include<string>
#include<iostream>
#include "util.hpp"
/* 网络地址类。负责创建具有指定IP地址和端口号的网络地址，用于唯一标识一个socket IO接口 */
class InetAddress
{
private:
    struct sockaddr_in addr;
    socklen_t addr_len;
public:
    InetAddress();
    InetAddress(std::string, uint16_t);

    void setAddr(const struct sockaddr_in _addr, const socklen_t _addr_len)
    {
        addr = _addr;
        addr_len = _addr_len; // 返回-1说明地址族无效。设置errno
    }

    inline sockaddr_in getAddr() const { return addr; }
    inline socklen_t getAddrLen() const { return addr_len; }
};
