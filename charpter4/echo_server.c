/*
 * @title 编写回声服务器
 * @user zys
 * @date 2026/2/26
 * @details 
 * 1.服务器端在同一时刻只与一个客户端相连，并提供回声服务
 * 2.服务器端依次向5个客户端提供服务并推出
 * 3.客户端接受用户输入的字符串数据并发送到客户端
 * 4.服务端将接受的字符串数据传回客户端，即“回声”
 * 5.服务端与客户端之间的字符串回声一直持续到客户端输入“Q”字符为止
 * @using ./[obj] [port]
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
    int serv_sock, client_sock;
    char buff[MAX_SIZE];
    int str_len;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;
    if(argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        handle_err("bind() error!");
    }
    if(listen(serv_sock, 5) == -1)
    {
        handle_err("listen() error!");
    }
    clnt_addr_sz = sizeof(clnt_addr);
    for(int i = 0; i < 5; i++)
    {
        client_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        if(client_sock == -1)
        {
            handle_err("accept() error!");
        }
        else
        {
            printf("Connected client %d \n", i+1);
        }
        while((str_len = read(client_sock, buff, MAX_SIZE))!=0)
        {
            write(client_sock, buff, str_len);
        }
        close(client_sock);
    }
    close(serv_sock);
    return 0;
}
void handle_err(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}