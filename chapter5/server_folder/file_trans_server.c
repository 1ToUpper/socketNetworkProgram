/*
 * @title 编写文件传输服务端
 * @user zys
 * @date 2026/3/5
 * @details 
 * 1.服务端等待客户端发送文件传输
 * 2.如果文件不存在则报错，断开连接
 * 3.客户端需要获取并发送文件大小，再循环读取文件到缓冲区，发送缓冲区中的内容
 * 4.服务端接收到文件大小后，进行循环读，读到缓冲区中
 * @using ./[obj] [port] [filename]
 */
#include "header.h" 

int main(int argv, char* argc[])
{
    if(argv != 3)
    {
        printf("Usage : %s <port> <filename>\n", argc[0]);
        exit(1);
    }
    FILE* file = fopen(argc[2], "wb");
    if(!file)
    {
        handle_err("fopen error -1!");
    }
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_stru, clnt_stru;
    socklen_t clnt_len = sizeof(clnt_stru);
    char send_buff[MAX_SIZE+1], recv_buff[MAX_SIZE+1];
    long recv_total_size = 0;
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
    {
        handle_err("socket error -1!");
    }
    serv_stru.sin_family = AF_INET;
    serv_stru.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_stru.sin_port = htons(atoi(argc[1]));
    if(-1 == bind(serv_sock, (struct sockaddr*)&serv_stru, sizeof(serv_stru)))
    {
        handle_err("bind error -1!");
    }
    if(-1 == listen(serv_sock, 3))              
    {
        handle_err("listen error -1!");
    }
    memset(&clnt_stru, 0, sizeof(clnt_stru));
    //for(int i = 0; i < 3; i++)
    {
        if((clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_stru, &clnt_len)) == -1)
        {
            handle_err("accept error -1!");
        }
        //先读取文件大小
        if(-1 == read(clnt_sock, recv_buff, MAX_SIZE))
        {
            handle_err("read file size error -1!");
        }
        //解析出文件大小
        recv_total_size = atol(recv_buff);
        printf("文件大小为%ld\n", recv_total_size);
        if(recv_total_size == 0)
        {
            handle_err("file size is zero!");
        }
        memset(send_buff, 0, sizeof(send_buff));
        sprintf(send_buff, "ok");
        if(-1 == write(clnt_sock, send_buff, sizeof(send_buff)))
        {
            handle_err("server is not prepared!");
        }
        ssize_t cur_recv_size = 0;
        ssize_t acc_recv_size = 0;
        while(acc_recv_size < recv_total_size)
        {
            cur_recv_size = read(clnt_sock, recv_buff, sizeof(recv_buff)-1);
            if(cur_recv_size <= 0)
            {
                printf("总接收数据大小%ld,接收错误，结束本次...\n", acc_recv_size);
                printf("已接收: %.2f%%\r",(double)acc_recv_size / recv_total_size * 100);
                break;
            }
            ssize_t fw_size = fwrite(recv_buff, 1, cur_recv_size, file);
            if(fw_size != cur_recv_size)
            {
                printf("文件写入错误: 期望写入%ld字节，实际写入%ld字节\n", cur_recv_size, fw_size);
                break;
            }
            acc_recv_size += cur_recv_size;
            printf("已接收: %.2f%%\r",(double)acc_recv_size / recv_total_size * 100);
            fflush(stdout);
            memset(recv_buff, 0, MAX_SIZE);
        }
        memset(send_buff, 0, sizeof(send_buff));
        sprintf(send_buff, "end");
        printf("send_buff:%s, size = %ld\n", send_buff, sizeof(send_buff));
        if(-1 == write(clnt_sock, send_buff, sizeof(send_buff)))
        {
            handle_err("file recv is not end!");
        }
        close(clnt_sock);
        if(acc_recv_size == recv_total_size)
        {
            printf("文件接收成功，共%ld字节\n", acc_recv_size);
        }
        else
        {
            printf("文件接收不完整: 期望%ld字节，实际%ld字节\n", recv_total_size, acc_recv_size);
        }
    }
    close(serv_sock);
    return 0;
}
