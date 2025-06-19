// server/src/Processes/ServerProcesses.cpp

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <errno.h>


#include "../../include/Sockets/sockets.hpp"
#include "../../include/Processes/ServerProcess.hpp"
#include "../../include/Sessions/WorkerSession.hpp"
#include "../../include/ConstantsAndVaribles.hpp"

// class Server

Server *Server::Start(EventSelector *sel, int port, int worker_com_channel[WORKERS_COUNT][STREAMS_COUNT])
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
    return new Server(sel, ls, worker_com_channel);
}

Server::Server(EventSelector *sel, int fd, int worker_channel[WORKERS_COUNT][STREAMS_COUNT]) 
    : FdHandler(fd, true)
    , the_selector(sel)
    , first(0)
    , current_worker(0)
{
    int i;
    for(i = 0; i < WORKERS_COUNT; i++)
    {
        item *p = new item;
        p->next = first;
        p->s = new WorkerSession(this, worker_channel[i][SOCKET_PARENT]);
        first = p;
        the_selector->Add(p->s);
    }
    the_selector->Add(this);    
}

Server::~Server()
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

void Server::Handle(bool re, bool we)
{
    if(!re)
        return;

    int sd;
    
    struct sockaddr_in addr;
    socklen_t len =  sizeof(addr);

    sd = accept(GetFd(), (struct sockaddr *) &addr, &len);
    if(sd == -1)
    {
        perror("accept filed");
        return;
    }

    current_worker = (current_worker + 1) % WORKERS_COUNT; 
    int i;
    item *tmp = first;
    for(i = 0; i < current_worker; i++) 
        tmp = tmp->next;

    int worker_socket = tmp->s->GetFd();

    char dummy = '\0';
    struct iovec io = {
        .iov_base = &dummy,
        .iov_len = 1
    };

    char cmsg_buf[CMSG_SPACE(sizeof(int))];
    memset(cmsg_buf, 0, sizeof(cmsg_buf));

    struct msghdr msg = {0};
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = cmsg_buf;
    msg.msg_controllen = sizeof(cmsg_buf);

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    *(int *)CMSG_DATA(cmsg) = sd;

    int res = sendmsg(worker_socket, &msg, 0);
    if(res < 0)
    {
        perror("sendmsg failed, пиздец иди нахуй");
    }
    close(sd);
}

void Server::ShutDownAllServer()
{
    the_selector->BreakLoop();
}
