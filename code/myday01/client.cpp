#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>  //这个头文件包含了<netinet/in.h>，不用再次包含了
#include<cstring>
#include<cstdio>
#include "./src/util.h"
#include<unistd.h>
#include<iostream>
using std::cout,std::endl,std::cin;
int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errorif(sockfd == -1, "socket 创建错误");
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    errorif(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr) == -1,"ip地址转换:");
    serv_addr.sin_port = htons(7777);
    errorif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect");

    while (1)
    {
        char buffer[1024];
        bzero(buffer, sizeof(buffer));
        cout << "输入要传到服务器的数据：";
        cin >> buffer;
        ssize_t write_bytes = send(sockfd, buffer, sizeof(buffer), 0);
        if(write_bytes == -1){          //write返回-1，表示发生错误
            cout <<"socket already disconnected, can't write any more!\n" <<endl;
            break;
        }
        bzero(&buffer, sizeof(buffer));       //清空缓冲区 
        ssize_t read_bytes = read(sockfd, buffer, sizeof(buffer));    //从服务器socket读到缓冲区，返回已读数据大小
        if (read_bytes > 0){
            cout << "服务器发送信息：" << buffer << endl;
        }
        else if (read_bytes == 0){
            cout << "服务器socket关闭" << endl;
            break;  
        }
        else{
            close(sockfd);
            errorif(true, "socket read error");
        }
    }

}
