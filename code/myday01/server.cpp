#include<sys/socket.h>
#include<arpa/inet.h>  //这个头文件包含了<netinet/in.h>，不用再次包含了
#include<cstring>
#include<cstdio>
#include "util.h"
int main(){
    struct sockaddr_in serv_addr;//socket所在服务器的地址(由ip地址和端口号标识)


    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errorif(sockfd == -1, "socket 创建");
    
    
    bzero(&serv_addr, sizeof(serv_addr)); //    确定对象被使用前已先被初始化。如果不清空，使用gdb调试器查看addr内的变量，会是一些随机值，未来可能会导致意想不到的问题。
    serv_addr.sin_family = AF_INET; // 设置地址族
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");// 设置IP地址
    serv_addr.sin_port = htons(8888); // Functions to convert between host and network byte order.计算机网络的字节序都是小端

   /* 为什么定义的时候使用专用socket地址（sockaddr_in）而绑定的时候要转化为通用socket地址（sockaddr），
    以及转化IP地址和端口号为网络字节序的inet_addr和htons等函数及其必要性，在游双《Linux高性能服务器编程》第五章第一节：socket地址API中有详细讨论。 */
    errorif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "bind"); // 将socket所在主机得网络地址与文件描述符绑定。
    errorif(listen(sockfd, SOMAXCONN) == -1, "listen"); // 监听套接字端口。处理来自客户端的请求。

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    errorif(clnt_sockfd == -1, "socket accept");
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
     


}
