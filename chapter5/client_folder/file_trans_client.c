/*
 * @title 编写文件传输服务端 
 * @user zys 
 * @date 2026/3/5 
 * @details 
 * 1.服务端等待客户端发送文件传输 
 * 2.如果文件不存在则报错，断开连接 
 * 3.客户端需要获取并发送文件大小，再循环读取文件到缓冲区，发送缓冲区中的内容 
 * 4.服务端接收到文件大小后，进行循环读，读到缓冲区中 
 * @using ./[obj] [ip] [port] [filename] 
 */
#include "header.h" 

int main(int argv, char* argc[])
{
    if(argv != 4)
    {
        printf("Usage : %s <ip> <port> <filename> \n", argc[0]);
        exit(1);
    }
    int serv_sock;
    struct sockaddr_in serv_stru;
    char send_buff[MAX_SIZE+1], recv_buff[MAX_SIZE+1];
    long file_size = 0;
    size_t bytes_read;
    long file_total_sent = 0;
    FILE *file = fopen(argc[3], "rb");
    if(!file)
    {
        handle_err("file open error -1!");
    }
    //获取文件长度
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("read file size = %ld\n", file_size);
    //建立连接
    memset(&serv_stru, 0, sizeof(serv_stru));
    serv_stru.sin_family = AF_INET;
    serv_stru.sin_addr.s_addr = inet_addr(argc[1]);
    serv_stru.sin_port = htons(atoi(argc[2]));
    if((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        handle_err("socket error -1!");
    }
    if(connect(serv_sock,(struct sockaddr*)&serv_stru, sizeof(serv_stru)) == -1)
    {
        handle_err("connect error -1!");
    }
    //先发送文件长度，收到ok后，再发送文件
    memset(send_buff, 0, sizeof(send_buff));
    memset(recv_buff, 0, sizeof(recv_buff));
    snprintf(send_buff, MAX_SIZE, "%ld", file_size);
    write(serv_sock, send_buff, sizeof(send_buff)-1);
    read(serv_sock, recv_buff, MAX_SIZE+1);
    //服务端回应可以发送数据
    printf("000 recv from server:%s\n", recv_buff);
    if(strcmp(recv_buff, "ok") != 0)
    {
        handle_err("strcmp ok error -1!");
    }
    while((bytes_read = fread(send_buff, 1, MAX_SIZE, file)) > 0)
    {
        ssize_t cur_send_size = write(serv_sock, send_buff, bytes_read);
        if(cur_send_size <= 0)
        {
            printf("发送失败...");
            break;
        }
        file_total_sent += cur_send_size;
        printf("已发送: %.2f%%\r",(double)file_total_sent / file_size * 100);
        fflush(stdout);
        memset(send_buff, 0, MAX_SIZE);
    }
    read(serv_sock, recv_buff, 4);
    printf("111 recv from server:%s\n", recv_buff);
    if(strcmp(recv_buff, "end") != 0)
    {
        handle_err("strcmp end error -1!");
    }
    close(serv_sock);
    fclose(file);
    printf("\n文件发送完成，共 %ld 字节\n", file_total_sent);
    return 0;
}