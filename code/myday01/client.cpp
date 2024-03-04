#include<sys/socket.h>
#include<arpa/inet.h>  //这个头文件包含了<netinet/in.h>，不用再次包含了
#include<cstring>
#include<cstdio>
#include "util.h"
int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errorif(sockfd == -1, "socket 创建错误");
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    errorif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect");
}
