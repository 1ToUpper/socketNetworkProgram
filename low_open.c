/*
 * @details 创建文件并写入一些文本内容
 * @user zys
 * @date 2026/1/19
 */

#include "header.h"

int main()
{
    int fd;
    char buf[] = "I'm creating new file and write something.";
    fd = open("data.txt", O_CREAT|O_RDWR|O_TRUNC);
    if(fd == -1)
    {
        error_handling("open() error!");
    }
    printf("file descriptor: %d\n", fd);
    if(write(fd, buf, sizeof(buf)) == -1)
    {
        error_handling("write() error!");
    }
    close(fd);
    return 0;
}