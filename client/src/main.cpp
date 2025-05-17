// client/src/main.cpp

#include <stdio.h>


#include "../include/Exception/exception.hpp"
#include  "../include/sockets.hpp"
#include "../include/chatclient.hpp"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Usage: ./client_char <IP> <PORT>\n");
        return 1;
    }

    EventSelector *selector = new EventSelector;
    ChatClient *client = new ChatClient::Start(selector, argv[1], argv[2]);
    
    if(!client)
    {
        perror("client");
        return 2;
    }

    selector->Run();

    return 0;
}
