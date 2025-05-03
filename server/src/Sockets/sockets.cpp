
#include <unistd.h>
#include "../include/Sockets/Sockets.hpp"


FdHandler::~FdHandler()
{
    if(own_fd)
        close(fd);
}

EventSelect::~EventSelect()
{
    if(fd_array)
        delete[] fd_array;
}

void EventSelect::Add(FdHandler *el)
{
    int i;
    int fd = el->GetFd();
    if(!fd_array)
    {
        fd_array_len = fd > 15 ? fd + 1 : 16;
        fd_array = new FdHandler*[fd_array_len];
        for(i = 0; i < fd_array_len; i++)
            fd_array[i] = 0;
        max_fd = -1;
    }
    if(fd_array_len <= fd)
    {
        FdHandler **tmp = new FdHandler[fd + 1];
        for(i = 0; i <= fd; i++)
            tmp[i] = i < fd_array_len ? fd_array[i] : 0;
        fd_array_len = fd + 1;
        delete[] fd_array;
        fd_array = tmp;
    }
    
    if(fd > max_fd)
        max_fd = fd;
    fd_array[fd] = el;
}

bool EventSelect::Remove(FdHendler *el)
{
    int fd = el->GetFd;
    if(fd >= fd_array_len || fd_array[fd] != el)
        return false;
    fd_array[fd] = 0;
    if(fd == max_fd)
    {
        while(max_fd >= 0 && !fd_array[max_fd])
            max_fd--;
    }
    return true;
}

void Run()
{
    
}
