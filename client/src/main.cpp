// client/src/main.cpp

#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
#include <cstring> 


#include  "../include/Sockets/sockets.hpp"

static int port = 7777; 

int main()
{
    ChatClient client;

    if(!client.Connected())
    {
       perror("client"); 
        return 1;
    }

    client.Start();

    return 0;
}
