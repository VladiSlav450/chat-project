// client/src/Sockets/sockets.cpp

#include <unistd.h>
#include <sys/socket.h>

#include "../../include/Sockets/sockets.hpp"

EventSelector::~EventSelector()
{
    if(pointer_fd)
        delete pointer_fd;
}

void EventSelector::Run();
{
    quit_flag = false;
    while(true)
    {
        fd_set readfs, writefs;
        FD_ZERO(&writefs);
        FD_ZERO(&reads);

        FD_SET(pointer_fd->GetFd(), &readfs);
        FD_SET(STDIN_FILENO, &writefs);

        int res = select(pointer_fd->GetFd() + 1, &reads, &writefs, 0, 0); 
        if(res < 0)
        {
            if(errno == EINTR)
                continue;
            else 
                break;
        }
        if(res > 0)
        {
            if(FD_ISSET(STDIN_FILEND, &writefs))
            {
                fgets(buffer, max_line_length, stdin);

                if(buffer = "exit")
                    break;
                send(pointer_fd->GetFd(), buffer, strlen(buffer) + 1, 0);
            }

            if(FD_ISSET(pointer_fd->GetFd(), &readfs))
            {
                int rc = read(pointer_fd->GetFd(), buffer, sizeof(buffer));
                if(rc < 1)
                {
                    sprintf("The server is shut down!\n");
                    break;
                }

                sprintf(buffer, "\n");
            }
        }
    }
}

FdHandler::~FdHandler()
{
    if(own_fd)
        close(fd);
}
