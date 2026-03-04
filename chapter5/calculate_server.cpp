/*
 * @title 编写简易计算器服务端
 * @user zys
 * @date 2026/2/27
 * @details 
 * 1.使用逆波兰表达式发送需要计算的式子
 * 2.只接受“+”，“-” ，“*”，“/” 四种运算符
 * 3.将计算结果返回给客户端
 * 4.考虑除数为0的情况
 * 5.使用栈计算后缀表达式
 * @using ./[obj] [port]
 */
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

const int MAX_SIZE = 1024;

template <typename T>
class stack
{
public:
    inline int get_length() { return elements.size(); }
    bool get_top(T& elem);
    bool pop_elem(T& elem);
    void push_elem(const T& elem);
private:
    std::vector<T> elements;
};
template <typename T>
bool stack<T>::get_top(T& elem)
{
    if(elements.size() == 0)
    {
        return false;
    }
    else 
    {
        elem = elements.at(elements.size() - 1);
        return true;
    }
}
template <typename T>
bool stack<T>::pop_elem(T& elem)
{
    if(elements.size() == 0)
    {
        return false;
    }
    else 
    {
        elem = elements.at(elements.size() - 1);
        elements.pop_back();
        return true;
    }
}
template <typename T>
void stack<T>::push_elem(const T& elem)
{
    elements.push_back(elem);
}
void handle_err(const char* message);
bool parse_postfix_expression(char* exp, int size, int& res);
int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    char recv_buff[MAX_SIZE+1], send_buff[MAX_SIZE+1];
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_adr_sz = sizeof(clnt_addr);
    if(argc != 2)
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }
    if((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        handle_err(string("socket error -1!").c_str());
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        handle_err(string("bind error -1!").c_str());
    }
    if(listen(serv_sock, 3) == -1)
    {
        handle_err(string("listen error -1!").c_str());
    }
    for(int i = 0; i < 3; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
        if(clnt_sock == -1)
        {
            handle_err(string("accept error -1!").c_str());
        }
        else
        {
            printf("Connected client %d \n", i+1);
        }
        read(clnt_sock, recv_buff, MAX_SIZE);
        int res = 0;
        memset(send_buff, 0, sizeof(send_buff));
        if(parse_postfix_expression(recv_buff, strlen(recv_buff), res))
        {
            sprintf(send_buff, "计算的结果是:%d", res);
        }
        else
        {
            strcpy(send_buff, "解析失败！");
        }
        write(clnt_sock, send_buff, sizeof(send_buff));
    }
    return 0;
}

void handle_err(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

bool parse_postfix_expression(char* exp, int size, int& res)
{
    stack<int> st;
    for(int i = 0; i < size; i++)
    {
        if(exp[i] >= '0' && exp[i] <= '9') 
        {
            st.push_elem(exp[i]-'0');
        }
        else 
        {
            int op1 = 0, op2 = 0;
            if(st.get_length() < 2) return false;
            else if(exp[i]=='+')
            {
                if(st.pop_elem(op2) && st.pop_elem(op1))
                {
                    res = op1 + op2;
                    st.push_elem(res);
                }
                else
                {
                    return false;
                }
            }
            else if(exp[i]=='-')
            {
                if(st.pop_elem(op2) && st.pop_elem(op1))
                {
                    res = op1 - op2;
                    st.push_elem(res);
                }
                else
                {
                    return false;
                }
            }
            else if(exp[i]=='*')
            {
                if(st.pop_elem(op2) && st.pop_elem(op1) && op2 != 0)
                {
                    res = op1 / op2;
                    st.push_elem(res);
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }
    if(st.get_length() > 1)
    {
        return false;
    }
    return true;
}