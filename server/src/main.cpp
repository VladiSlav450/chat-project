// server/src/main.cpp

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>


#include "../include/Chat/chat.hpp"
#include "../include/Sockets/sockets.hpp"

static int the_server_port = 7777;

int main()
{
    int pipes[WORKERS_COUNT][STREAMS_COUNT]; // [0] - read, [1] - write
    int worker_pids[WORKERS_COUNT];

    int i;
    for(i = 0; i < WORKERS_COUNT; i++)
    {
        if(pipe(pipes[i]) == -1)
        {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
        close(pipes[i][READ]);
    }
   for(i = 0; i < WORKERS_COUNT; i++)
    {
        worker_pids[i] = fork();
        if(worker_pids[i] == -1)
        {
            perror("fork failed");
            exit(EXIT_FAILURE); 
        }

        if(worker_pids[i] == 0)
        {
            close(pipes[i][WRITE]);
            int j;
            for(j = 0; j < WORKERS_COUNT; j++)
                if(j != i)
                    close(pipes[i][READ]); 
            close(pipes[i][WRITE]);
            WorkerServer::worker_func_main(i, pipes); 
            exit(0);
        }
    }

    EventSelector *selector = new EventSelector;
    ChatServer *server = ChatServer::Start(selector, the_server_port, pipes);
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
