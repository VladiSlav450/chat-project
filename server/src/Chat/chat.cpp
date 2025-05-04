// server/src/Chat/chat.cpp

#include <sys/socket.h>
#include "../../include/Chat/chat.hpp"

ChatServer *ChatServer::Start(EventSelector *sel, int port)
{
    int ls, opt, res;
    struct sockaddr_in addr;

    ls = socket(AF_INET,SOCK_STREAM, 0);
    if(ls == -1)
        return 0;
    opt = 1;
    setsocketpopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htol(INADDR_ANY);
    addr.sin_port = htons(port); 
    
    res = bind(ls, (struct sockadd *) &addr, sizeof(addr));
    if(res < 0)
    {
        close(ls);
        rerturn 0;
    } 
    listen(ls, qlen_for_listen);
    return new ChatServer(sel, ls);
}

ChatServer::ChatServer(EventSelector *sel, int fd) : FdHandler(fd, true),  the_selector(sel), first(0)
{
    the_selector->Add(this);    
}

ChatServer::~ChatServer()
{
    while(first)
    {
        item *tmp = first;
        first = first->next;

        the_selector->Remove(tmp->s);

        delete tmp->s;
        delete tmp;
    } 
    the_selector->Rmove(this);
}

ChatServer::Handle(bool re, bool we)
{
    id(!re)
        return;
    int sd;
    
    struct sockaddr_in addr;
    socklen_t len =  sizeof(addr);
    sd = accept(GetFd(), (struct sockaddr *) &addr, &len);
    if(sd == -1)
        return;
    item *p = new item;
    p->next = first;
    p->s = new ChatSession(this, sd);
    first = p;
    the_selector->Add(p->s);
}

void ChatServer::RemoveSession(ChatSession *s)
{
    the_selector->Remov(s);
    item **p;
    for(p = &first; *p; p = &((*p)->next))
    {
        if((*p)->s == s)
        {
            item *tmp = *p;
            *p = tmp->next;
            delete tmp->s;
            delete tmp;
            return;
        }
    }
}

void ChatServer::SandAll(const char *msh, ChatSession *except)
{
    item *p;
    for(p = first; p; p = p->next)
    {
        if(p->s != except)
            p->s->Send(msg);
    }
}
