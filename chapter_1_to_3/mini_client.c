#include "header.h"

/*
 * @details client端1.0程序发送一段数据
 * @user zys
 * @date 2026/1/27
 * @using ./[obj] [ip 127.0.0.1] [port]
 */

 
int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[1024];
    int str_len = 0;
    int idx = 0, read_len = 0;

    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        error_handling("socket() error");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling("connect() error");
    }

    while(read_len = read(sock, &message[idx++], 1))
    {
        printf("read_len:%d\n", read_len);
        if(read_len == -1)
        {
            error_handling("read() error!");
        }
        str_len += read_len;
        //modify:延时调用读取
        for(int i = 0;i<3000;i++)
        {
            if(i%1000 == 0)
            {
                printf("Wait time %d \n", i);
            }
        }
    }
    printf("Message from server: \n");
    for(int i=0;i<str_len;i++)
    {
        if(message[i]=='\0')
        {
            continue;
        }
        else
        {
            printf("%c", message[i]);
        }
    }
    printf("\nFunction read call count:%d\n",str_len);
    close(sock);
    return 0;
}