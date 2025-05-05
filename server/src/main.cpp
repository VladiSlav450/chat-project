// server/src/main.cpp

#include <stdio.h>
#include <errno.h>
#include "../include/Chat/chat.hpp"
#include "../include/Sockets/sockets.hpp"

static int port = 7777;

int main()
{
    EventSelector *selector = new EventSelector;
    ChatServer *server = ChatServer::Start(selector, port);
    if(!server)
    {
        perror("server");
        return 1;
    }

    selector->Run();
    return 0;
}
