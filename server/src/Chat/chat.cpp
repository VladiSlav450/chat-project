// server/src/Chat/chat.cpp

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../../include/Chat/chat.hpp"

ChatSession::ChatSession(ChatServer *a_master, int fd) : FdHandler(fd, true), buf_used(0), ignoring(false), name(0), the_master(a_master)
{
    Send("Your name please: "); 
}

ChatSession::~ChatSession()
{
    if(name)
        delete[] name;
}

void ChatSession::Send(const char *msg)
{
    write(GetFd(), msg, strlen(msg));    
}

void ChatSession::Handle(bool re, bool we)
{
    if(!re)
        return;
    if(buf_used >= (int)sizeof(buffer))
    {
        buf_used = 0;
        ignoring = true;
    }
    if(ignoring)
        ReadAndIgnore();
    else
        ReadAndCheck();
}

void ChatSession::ReadAndIgnore()
{
    int rc = read(GetFd(), buffer, sizeof(buffer));
    if(rc < 1)
    {
        the_master->RemoveSession(this);
        return;
    }

    int i;
    for(i = 0; i < rc; i++)
    {
        if(buffer[i] == '\n')
        {
            int rest = rc - i - 1;
            if(rest > 0)
                memmove(buffer, buffer + i + 1, rest);

            buf_used = rest;
            ignoring = false;
            CheckLines();
            return;
        }
    } 
}

void ChatSession::ReadAndCheck()
{
    int rc = read(GetFd(), buffer + buf_used, sizeof(buffer) - buf_used);
    if(rc < 1)
    {
        if(name)
        {
            int len = strlen(name);
            char *lmsg = new char[len + sizeof(left_msg) + 2];
            sprintf(lmsg, "%s%s\n", name, left_msg);
            the_master->SendAll(lmsg, this);
            delete[] lmsg;
        }
        the_master->RemoveSession(this);
        return;
    }
    buf_used += rc;
    CheckLines();
}

void ChatSession::CheckLines()
{
    if(buf_used < 0)
        return;
    int i;
    for(i = 0; i < buf_used; i++)
    {
        if(buffer[i] == '\n')
        {
            if(i > 0 && buffer[i - 1] == '\r')
                buffer[i - 1] = 0;
            ProcessLine(buffer);
            int rest = buf_used - i - 1;
            memmove(buffer, buffer + i + 1, rest);
            buf_used = rest;
            CheckLines();
            return;
        }
    }
}

void ChatSession::ProcessLine(const char *str)
{
    int len = strlen(str);
    if(!name)
    {
        name = new char[len + 1];
        strcpy(name, str);
        char *wmsg = new char[len + sizeof(welcom_msg) + 2];
        sprintf(wmsg, "%s%s\n", welcom_msg, name);
        Send(wmsg);
        delete[] wmsg;
        char *emsg = new char[len + sizeof(entered_msg) + 2];

        sprintf(emsg, "%s%s\n", name, entered_msg);
        the_master->SendAll(emsg, this);
        delete[] emsg;
        return;
    }

    int nl = strlen(name);
    char *msg = new char[nl + len + 5];
    sprintf(msg, "<%s> %s\n", name, str);

    the_master->SendAll(msg, this);
    delete[] msg;
}

ChatServer *ChatServer::Start(EventSelector *sel, int port)
{
    int ls, opt, res;
    struct sockaddr_in addr;

    ls = socket(AF_INET, SOCK_STREAM, 0);
    if(ls == -1)
        return 0;
    
    opt = 1;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port); 
    
    res = bind(ls, (struct sockaddr *) &addr, sizeof(addr));
    if(res == -1)
    {
        close(ls);
        return 0;
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
    the_selector->Remove(this);
}

void ChatServer::Handle(bool re, bool we)
{
    if(!re)
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
    the_selector->Remove(s);
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

void ChatServer::SendAll(const char *msg, ChatSession *except)
{
    item *p;
    for(p = first; p; p = p->next)
    {
        if(p->s != except)
            p->s->Send(msg);
    }
}
