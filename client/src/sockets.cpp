// client/src/sockets.cpp

#include <FL/Fl.H>

#include <sys/socket.h>         
#include <netinet/in.h>         
#include <arpa/inet.h>          
#include <unistd.h>            
#include <fcntl.h>      
#include <cerrno>


#include "../include/sockets.hpp"

// class FdHandler
FdHandler::~FdHandler()
{
    if(own_fd_)
        close(own_fd_);
}

// class EventSelector

EventSelector::~EventSelector()
{
    while(first)
    {
        Key_Array *tmp = first;
        first = first->next;
        delete tmp;
    }
}

void EventSelector::Add(FdHandler *el)
{
    int fd = el->GetFd();

    fd_array[fd] = el;

    Key_Array *tmp = new Key_Array;
    tmp->value = fd;
    tmp->next = first;
    first = tmp;

    if(max_fd < fd)
        max_fd = fd;
}

bool EventSelector::Remove(FdHandler *el)
{
    int fd = el->GetFd();
    if(el != fd_array[fd])
        return false;

    bool mx = false;
    if(fd == max_fd)
    {
        max_fd = -1;
        mx = true;
    }

    Key_Array **tmp;
    for(tmp = &first; *tmp; tmp = &(*tmp)->next)
    {
        if((*tmp)->value == fd)
        {
            fd_array[fd] = 0;
            Key_Array *delete_to = *tmp;

            *tmp = delete_to->next;
            delete delete_to;
            if(!mx)
                return true;
            continue;
        }
        if(mx)
        {
            if((*tmp)->value > max_fd && (*tmp)->value < fd)
                max_fd = (*(tmp))->value;
        }
    }
    return true;
}

void EventSelector::Run()
{
    quit_flag = false;

    do {
        fd_set rds, wrs;

        FD_ZERO(&rds);
        FD_ZERO(&wrs);

        for(Key_Array *tmp = first; tmp; tmp = tmp->next)
        {
            int fd_key = tmp->value;
            FdHandler *handler = fd_array[fd_key];

            if(handler->WantRead())
                FD_SET(fd_key, &rds);

            if(handler->WantWrite())
                FD_SET(fd_key, &wrs);
        }

        int res = select(max_fd + 1, &rds, &wrs, 0, 0);
        if(res < 0)
        {
            if(errno == EINTR)
                continue;
            else
                break;
        }

        if(res > 0)
        {
            Key_Array *tmp = first;
            while(tmp)
            {
                Key_Array *next = tmp->next;

                int fd_key = tmp->value;
                FdHandler *handler = fd_array[fd_key];

                if(handler)
                {
                    bool r = FD_ISSET(fd_key, &rds);
                    bool w = FD_ISSET(fd_key, &wrs);
                    if(r || w)
                        handler->Handle(r, w);
                }
                tmp = next;
            }
        }
    } while(!quit_flag);
}
