/*
 * @details ip地址接口转换
 * @user zys
 * @date 2026/2/22
 */
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    char* addr = "1.2.3.4";
    char* addr_err = "1.2.3.256"; //limit:[1,255]

    //1.使用inet_addr接口进行转换
    unsigned long conv_addr=inet_addr(addr);
    if(conv_addr == INADDR_NONE)
    {
        printf("Error occured! \n");
    }
    else 
    {
        printf("Network ordered integer addr: %#lx \n", conv_addr);
    }
    conv_addr=inet_addr(addr_err);  //Error!
    if(conv_addr == INADDR_NONE)
    {
        printf("Error occured! \n");
    }
    else 
    {
        printf("Network ordered integer addr: %#lx \n", conv_addr);
    }

    //2.使用inet_aton接口进行转换
    char* addr_2 = "192.168.1.121";
    struct sockaddr_in addr_inet;
    if(!inet_aton(addr_2, &addr_inet.sin_addr))
    {
        printf("Conversion error");
    }
    else
    {
        printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);
    }

    //3.使用inet_ntoa接口进行转换
    struct sockaddr_in addr1, addr2;
    char* str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x10203040);
    addr2.sin_addr.s_addr = htonl(0x10101010);
    str_ptr=inet_ntoa(addr1.sin_addr);
    strcpy(str_arr, str_ptr);
    printf("Dotted-Decimal notation1: %s \n", str_ptr);

    str_ptr=inet_ntoa(addr2.sin_addr);
    printf("Dotted-Decimal notation2: %s \n", str_ptr);
    printf("Dotted-Decimal notation3: %s \n", str_arr);
    return 0;
}