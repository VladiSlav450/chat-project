// server/src/sockets/sockets.cpp

#include <cerrno>
#include <sys/select.h>
#include <unistd.h>
#include "../../include/Sockets/sockets.hpp"


// class FdHandler
FdHandler::~FdHandler()
{
    if(own_fd)
        close(fd);
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
            Key_Array *delete_to = *tmp;
            *tmp = (*tmp)->next;
            delete delete_to;

            fd_array[fd] = 0;
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
        int i;
        fd_set rds, wrs;
        FD_ZERO(&rds);
        FD_ZERO(&wrs);
        for(i = 0; i <= max_fd; i++)
        {
            if(fd_array[i])
            {
                if(fd_array[i]->WantRead())
                    FD_SET(i, &rds);
                if(fd_array[i]->WantWrite())
                    FD_SET(i, &wrs);
            }
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
            for(i = 0; i <= max_fd; i++)
            {
                if(!fd_array[i])
                    continue;
                bool r = FD_ISSET(i, &rds);
                bool w = FD_ISSET(i, &wrs);
                if(r || w)
                    fd_array[i]->Handle(r, w);
            }
        }
    } while(!quit_flag);
}
