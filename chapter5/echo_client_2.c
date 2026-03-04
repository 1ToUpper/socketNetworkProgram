/*
 * @title 编写回声客户端
 * @user zys
 * @date 2026/2/27
 * @details 
 * 改进：如果数据较大，可能无法接收到全部数据，使用循环读取
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
    int recv_len = 0, send_len = 0;
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
        send_len = write(sock, buff, strlen(buff));
        recv_len = 0;
        while(recv_len < send_len)
        {
            int rl = read(sock, &buff[recv_len], MAX_SIZE-1);
            if(rl == -1)
            {
                handle_err("Read error!");
            }
            recv_len += rl;
        }
        buff[recv_len] = 0;
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