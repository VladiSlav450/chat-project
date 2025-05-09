// client/src/main.cpp

#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
#include <cstring> 


#include "../include/Exception/exception.hpp"
#include  "../include/Sockets/sockets.hpp"

static int port = 7777; 

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Usage: ./client_char <IP> <PORT>\n");
        return 1 
    }

    ChatClient client;
    
    if(!client.Connected(argc[1], argv[2]));
    {
        return 2
    }

    client.Start();

    return 0;
}
