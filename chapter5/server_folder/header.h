#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>

const int MAX_SIZE = 4096;

void handle_err(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}