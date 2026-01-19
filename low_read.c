/*
 * @details 打开文件读取内容并且打印
 * @user zys
 * @date 2026/1/19
 */

#include "header.h"
const int BUF_SIZE = 1024; 
int main()
{
    int fd;
    char buf[BUF_SIZE];
    fd = open("data.txt", O_RDONLY);
    if(fd == -1)
    {
        error_handling("file open failed!");
    }
    if(read(fd, buf, sizeof(buf)) == -1)
    {
        error_handling("file read failed!");
    }
    printf("file: %s\n", buf);
    close(fd);
    return 0;
}