#include "InetAddress.h"
#include<cstring>

InetAddress::InetAddress() : addr_len(sizeof(addr)) {
    bzero(&addr, addr_len);
}

InetAddress::InetAddress(const char *p, uint16_t port) : addr_len(sizeof(addr)){
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    int s = inet_pton(AF_INET, p, &addr.sin_addr.s_addr);
    if (s == 0)
        cout << "Not in presentation format" << endl;
    errorif(s==-1, "inet_pton");
}

void InetAddress::setInetAddr(sockaddr_in _addr, socklen_t _addr_len)
{
    addr = _addr;
    addr_len = _addr_len;
}

sockaddr_in InetAddress::getAddr()
{
    return addr;
}
socklen_t InetAddress::getAddr_len()
{
    return addr_len;
}
