/*
 * @title 编写回声客户端
 * @user zys
 * @date 2026/2/26
 * @details 
 * 1.服务器端在同一时刻只与一个客户端相连，并提供回声服务
 * 2.服务器端依次向5个客户端提供服务并推出
 * 3.客户端接受用户输入的字符串数据并发送到客户端
 * 4.服务端将接受的字符串数据传回客户端，即“回声”
 * 5.服务端与客户端之间的字符串回声一直持续到客户端输入“Q”字符为止
 * @using ./[obj] [ip(127.0.0.1)] [port]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_SIZE 1024
void handle_err(char* message);
int main(int argc, char* argv[])
{
    int sock;
    char buff[MAX_SIZE];
    int str_len;
    struct sockaddr_in serv_addr;
    if(argc != 3)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        handle_err("Connected error!");
    }
    else
    {
        puts("Connect waiting......");
    }
    while(1)
    {
        fputs("Send message(Q to quit): ", stdout);
        fgets(buff, MAX_SIZE, stdin);
        if(!strcmp(buff, "q\n") || !strcmp(buff, "Q\n"))
        {
            break;
        }
        write(sock, buff, strlen(buff));
        str_len = read(sock, buff, MAX_SIZE-1);
        buff[str_len] = 0;
        printf("Message received from server: %s", buff);
    }
    close(sock);
    return 0;
}
void handle_err(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}