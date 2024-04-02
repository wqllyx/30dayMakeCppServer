#include "InetAddress.hpp"
#include<arpa/inet.h>
#include<string>
#include<strings.h>
#include "util.hpp"
#include<iostream>
using namespace std;

InetAddress::InetAddress() : addr_len(sizeof(addr)) {
    bzero(&addr, addr_len);
}

InetAddress::InetAddress(string ip, uint16_t port) : addr_len(sizeof(addr))
{
    bzero(&addr, addr_len);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    int s = inet_pton(AF_INET, ip.data(), &addr.sin_addr.s_addr);
    if (s == 0)  // 返回0,说明IP地址格式不对。
        cout << "Not in presentation format" << endl;
    errorif(s == -1, "inet_pton");// 返回-1说明地址族无效。设置errno
}
