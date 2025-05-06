// client/src/Sockets/sockets.cpp

#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
#include <cstring> 


#include "../../include/Sockets/sockets.hpp"

static const char server_disconnected[] = "[Server dissconnected!]";
static const char server_connected[] = "[Server connected!]";

ChatClient::~ChatClient()
{
    if(is_connected)
        close(sock);
}

bool ChatClient::Connected(const char *ip, int port)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        return false;

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip);

    if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        close(sock);
        return false;
    }

    is_connected = true;
    printf("%s\n", server_connected);

    return is_connected;
}

void ChatClient::Send(const char *massege)
{
    if(!is_connected)
        return;

    send(sock, massege, strlen(massege) + 1, 0);
}

void ChatClient::Read(char *str, size_t size)
{
    if(!is_connected)
    {
        str[0] = '\0';
        return;
    }

    int i;

    int bytes = read(sock, str, size);
    if(bytes < 1)
    {
        is_connected = false;
        strncpy(str, server_disconnected, size);
        return;
    }

    for(i = 0; i < bytes; i++)
    {
        if(str[i] == '\n')
            str[i] = 0;
    }

    return;
}
