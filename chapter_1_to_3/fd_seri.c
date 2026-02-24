/*
 * @details 打开三个文本描述符号观察系统如何分配的（0：标准输入，1：标准输出，2：标准错误）
 * @user zys
 * @date 2026/1/19
 */

#include "header.h"

int main()
{
    int fd1, fd2, fd3;
    fd1 = socket(PF_INET, SOCK_DGRAM, 0);
    fd2 = socket(PF_INET6, SOCK_STREAM, 0);
    fd3 = open("data.txt", O_RDONLY);
    printf("fd1 = %d, fd2 = %d, fd3 = %d\n", fd1, fd2, fd3);
    return 0;
}