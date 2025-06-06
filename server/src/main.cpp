// server/src/main.cpp

#include <stdio.h>
#include <errno.h>
#include "../include/Chat/chat.hpp"
#include "../include/Sockets/sockets.hpp"

static int the_server_port = 7777;

int main()
{
    int worker_pipes[3][2];
    int worker_pides[3];

    int i;
    for(i = 0; i < 3; i++)
    {
        if(pipe(worker_pipes[i]) == -1)
        {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }

        worker_pides[i] = fork();
        if(worker_pides[i] == -1)
        {
            perror("fork failed");
            exit(EXIT_FAILURE): 
        }

        if(worker_pides[i] == 0)
        {
            // Тут порождённый процесс сам создаёт клиенетаские сесси и отслеживает их
            worker_func_main(worker_pipes[i][0]); 
            exit(0);
        }
        else
            close(worker_pides[i][0]);
    }

    EventSelector *selector = new EventSelector;
    ChatServer *server = ChatServer::Start(selector, the_server_port, worker_pipes);
    if(!server)
    {
        perror("server start failed");
        return 1;
    }
    selector->Run();

    for(i = 0; i < 3; i++)
    {
        close(worker_pides[i][1]);
        kill(worker_pides[i], SIGTERM);
    } 
    return 0;
}


#if 0
int main()
{
    EventSelector *selector = new EventSelector;
    ChatServer *server = ChatServer::Start(selector, the_server_port);
    if(!server)
    {
        perror("server");
        return 1;
    }

    selector->Run();
    return 0;
}
#endif
