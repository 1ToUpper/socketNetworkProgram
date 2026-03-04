/*
 * @title 编写简易计算器客户端
 * @user zys
 * @date 2026/2/27
 * @details 
 * 1.使用逆波兰表达式发送需要计算的式子
 * 2.只接受“+”，“-” ，“*”，“/” 四种运算符
 * 3.所有的符号和数字都使用一个字符存储
 * 4.将计算结果返回给客户端
 * 5.考虑除数为0的情况
 * 6.使用栈计算后缀表达式
 * @using ./[obj] [port]
 */
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

const int MAX_SIZE = 1024;

void handle_err(const char* message);
int main(int argc, char *argv[])
{
    int serv_sock;
    char recv_buff[MAX_SIZE+1], send_buff[MAX_SIZE+1];
    struct sockaddr_in serv_addr;
    if(argc != 3)
    {
        printf("Usage : %s <ip> <port> \n", argv[0]);
        exit(1);
    }
    if((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        handle_err(string("socket error -1!").c_str());
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        handle_err(string("connect error -1!").c_str());
    }
    else
    {
        std::cout << "connected success.."<<std::endl;
    }
    std::cout << "输入后缀表达式:"<<std::endl;
    std::cin >> send_buff;
    write(serv_sock, send_buff, sizeof(send_buff));
    read(serv_sock, recv_buff, MAX_SIZE);
    std::cout<<"接收的字符:"<<recv_buff<<std::endl;
    return 0;
}

void handle_err(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}