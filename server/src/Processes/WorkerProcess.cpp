// server/src/Processes/WorkerProcess.cpp

#include <sys/socket.h>
#include <sys/uio.h>
#include <string.h>
#include <stdio.h>


#include "../../include/Sockets/sockets.hpp"
#include "../../include/Processes/WorkerProcess.hpp"

// class WorkerServer

void WorkerServer::worker_func_main(int my_idx, int socket_channel[WORKERS_COUNT][STREAMS_COUNT], const char *path_config)
{
    EventSelector *selector = new EventSelector;  
    RouteConfigFile *config = new RouteConfigeFile(path_config);
    WorkerServer *workserv = new WorkerServer(selector, my_idx, socket_channel, config);

    selector->Run();

    delete workserv;
    delete config;
    delete selector;
}

WorkerServer::WorkerServer(EventSelector *sel, int idx, int socket_channel[WORKERS_COUNT][STREAMS_COUNT], RouteConfigFile *config) 
    : FdHandler(socket_channel[idx][SOCKET_CHILD], true)
    , the_selector(sel)
    , configfile(config)
    , first(0)
    , my_index(idx)
{
    int i, j;
    for(i = 0; i < WORKERS_COUNT; i++)
    {
        for(j = 0; j < STREAMS_COUNT; j++)
        {
            worker_com_channel[i][j] = socket_channel[i][j];
        }
    }

    the_selector->Add(this); 
}

WorkerServer::~WorkerServer()
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

void WorkerServer::RemoveSession(ClientSession *s)
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

char *WorkerServer::GetNumberUsersOnline()
{
    int i;
    int count_users = 0;
    item *tmp;
    for(tmp = first; tmp; tmp = tmp->next)
    {
        if(tmp->s->name)
            count_users++;
    } 

    i = 0;
    int count = count_users;
    while(count > 0)
    {
        count = count / 10;
        i++;
    } 

    char *msg = new char[strlen(server_commands) + i + 16]; 
    sprintf(msg, "%s online %d users\n", server_commands, count_users);

    return msg;
    
}

char *WorkerServer::GetNameUsersOnline()
{
    size_t len = max_line_length + 1;
    char *buf_user_online = new char[len];
    size_t serv_com_len = strlen(server_commands); 
    strcpy(buf_user_online, server_commands);
    buf_user_online[serv_com_len++] = ' ';
    buf_user_online[serv_com_len] = 0;
    size_t buf_used = serv_com_len;
    bool no_users = false;

    item *tmp;
    for(tmp = first; tmp; tmp = tmp->next)
    {
        if(!tmp->s->name)
            continue;
        no_users = true;
        size_t name_len = strlen(tmp->s->name);

        if(len < buf_used + name_len + 2)
        {
            len = name_len + len * 2;
            char *tmp = new char[len];
            memcpy(tmp, buf_user_online, buf_used);
            delete[] buf_user_online;

            buf_user_online = tmp;
        }

        memcpy(buf_user_online + buf_used, tmp->s->name, name_len);
        buf_used += name_len;
        buf_user_online[buf_used++] = '\n';
    }
    if(!no_users)
    {
        strcpy(buf_user_online + buf_used, "here no users(\n");
        buf_used += 15;
    }
    buf_user_online[buf_used] = 0;
    return buf_user_online;
}

const char *WorkerServer::IsNameUnique(const char *str)
{
    item *tmp;
    for(tmp = first; tmp; tmp = tmp->next)
    {
        if(!tmp->s->name)
            continue;

        if(strcmp(tmp->s->name, str) == 0)
            return name_already_take_msg;        
    } 
    return 0;
}

void WorkerServer::Handle(bool r, bool w)
{
    if(!r)
        return;

    char *msg_buffer = new char[max_line_length+1];

    struct iovec io = {
        .iov_base = msg_buffer,
        .iov_len = max_line_length + 1
    };

    char cmsg_buf[CMSG_SPACE(sizeof(int))];

    struct msghdr msg = {0};
    msg.msg_iov = &io;
    msg.msg_iovlen = 1; 
    msg.msg_control = cmsg_buf;
    msg.msg_controllen = sizeof(cmsg_buf);


    int res = recvmsg(GetFd(), &msg, 0);
    if(res < 0)
    {
        perror("recvmsg failed");
        return; 
    }

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);

    // проверям есть ли специальные данные в виде дескриптера
    if(cmsg && cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS)
    {
        // получение  дескриптера клиента от главного процесса
        int sd = *(int *)CMSG_DATA(cmsg); 

        item *p = new item;
        p->next = first;
        p->s = new ClientSession(this, sd);
        first = p;

        the_selector->Add(p->s);
    }
    else
    {
        // значит это сообщение
        msg_buffer[res] = '\0'; 
        if(strcmp(msg_buffer, "<server command> server shutdown\n") == 0)
        {
            SendAllinTheWorkerProcess(msg_buffer);
            the_selector->BreakLoop();
        }
        else
            SendAllinTheWorkerProcess(msg_buffer);
    }

    delete[] msg_buffer;    
}

void WorkerServer::SendAll(const char *msg, ClientSession *except)
{
    SendAllinTheWorkerProcess(msg, except);
    struct iovec io = {
        .iov_base = (void*)msg,
        .iov_len = strlen(msg) + 1
    };

    struct msghdr hdr = {0};
    hdr.msg_iov = &io;
    hdr.msg_iovlen = 1;
    hdr.msg_control = NULL;
    hdr.msg_controllen = 0;

    int i;
    for(i = 0; i < WORKERS_COUNT; i++)
    {
        if(i != my_index)
        {
            int res = sendmsg(worker_com_channel[i][SOCKET_PARENT], &hdr, MSG_NOSIGNAL); 
            if(res < 0)
                perror("sendmsg other worker failed");
        }
    }  
}

void WorkerServer::SendAllinTheWorkerProcess(const char *msg, ClientSession *except)
{
    item *p;
    for(p = first; p; p = p->next)
    {
        if(p->s != except)
            p->s->Send(msg);
    }
}

void WorkerServer::SendServer(const char *msg)
{
    struct iovec io = {
        .iov_base = (void*)msg,
        .iov_len = strlen(msg) + 1
    };

    struct msghdr hdr = {0};
    hdr.msg_iov = &io;
    hdr.msg_iovlen = 1;
    hdr.msg_control = NULL;
    hdr.msg_controllen = 0;
    int res = sendmsg(GetFd(), &hdr, MSG_NOSIGNAL);
    if(res < 0)
        perror("sendmsg to main process failed");
}

void WorkerServer::ShutDownAllServer()
{
    char *msg = new char[strlen(server_commands) + strlen(server_shutdown) + 3];
    sprintf(msg, "%s %s\n", server_commands, server_shutdown);

    SendAll(msg);  
    SendServer(msg);
    the_selector->BreakLoop();
    delete[] msg;
}

char *WorkerServer::FindPathForSendFile(const char *path)
{
    if(!configfile)
    {
        printf("Not file configuration\n");
        return 0;
    }
        
    return configfile->Resolve(path);
}


