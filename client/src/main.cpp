// client/src/main.cpp

#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
#include <cstring> 


#include "../include/Exception/exception.hpp"
#include  "../include/Sockets/sockets.hpp"

static int port = 7777; 

int main()
{
    ChatClient client;

    client.Connected();

    client.Start();

    return 0;
}
