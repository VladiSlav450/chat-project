// server/src/main.cpp

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>


#include "../include/Processes/ServerProcess.hpp"
#include "../include/Processes/WorkerProcess.hpp"
#include "../include/ConstantsAndVaribles.hpp"
#include "../include/Sockets/sockets.hpp"


static int the_server_port = 7777;

int main()
{
    int worker_com_channel[WORKERS_COUNT][STREAMS_COUNT]; // [0] - parent, [1] - child
    int worker_pids[WORKERS_COUNT];

    int i;
    for(i = 0; i < WORKERS_COUNT; i++)
    {
        if(socketpair(AF_UNIX, SOCK_STREAM, 0, worker_com_channel[i]) == -1)
        {
            perror("socketpair failed");
            return 1;
        }
    }
    for(i = 0; i < WORKERS_COUNT; i++)
    {
        worker_pids[i] = fork();
        if(worker_pids[i] == -1)
        {
            perror("fork failed");
            return 1; 
        }

        if(worker_pids[i] == 0)
        {
            close(worker_com_channel[i][SOCKET_PARENT]);
            worker_com_channel[i][SOCKET_PARENT] = -1;
            int j;

            for(j = 0; j < WORKERS_COUNT; j++)
            {
                if(j != i)
                {
                    close(worker_com_channel[j][SOCKET_CHILD]); 
                    worker_com_channel[j][SOCKET_CHILD] = -1;
                }
            }
            WorkerServer::worker_func_main(i, worker_com_channel, PATHCONFIGFILE); 
            exit(0);
        }
    }

    for(i = 0; i < WORKERS_COUNT; i++)
    {
        close(worker_com_channel[i][SOCKET_CHILD]);
        worker_com_channel[i][SOCKET_CHILD] = -1;
    }
    EventSelector *selector = new EventSelector;
    Server *server = Server::Start(selector, the_server_port, worker_com_channel);
    if(!server)
    {
        perror("server start failed");
        return 1;
    }
    selector->Run();

    int pid_wait;
    while((pid_wait = wait(NULL)) != -1) {}
    delete server;
    delete selector;
    return 0;
}
