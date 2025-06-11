// server/src/Chat/chat.cpp

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/socket.h>


#include "../../include/Chat/chat.hpp"

ClientSession::ClientSession(WorkerServer *a_master, int fd) 
    : FdHandler(fd, true), buf_used(0), ignoring(false), name(0)
    , the_master(a_master), current_state(fsm_ClientState::fsm_NewConnected)
    , need_to_delete(false)
{
    Send("Your name please: "); 
}

ClientSession::~ClientSession()
{
    if(name)
        delete[] name;
}

void ClientSession::Send(const char *msg)
{
    write(GetFd(), msg, strlen(msg));    
}

void ClientSession::Handle(bool re, bool we)
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

    if(need_to_delete)
        DisconnectedClient();
}

void ClientSession::ReadAndIgnore()
{
    int rc = read(GetFd(), buffer, sizeof(buffer));
    if(rc < 1)
    {
        need_to_delete = true;
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

void ClientSession::ReadAndCheck()
{
    int rc = read(GetFd(), buffer + buf_used, sizeof(buffer) - buf_used);
    if(rc < 1)
    {
        need_to_delete = true; 
        return;
    }
    buf_used += rc;
    CheckLines();
}

void ClientSession::CheckLines()
{
    while(buf_used > 0 && !need_to_delete)
    {
        int i;
        for(i = 0; i < buf_used; i++)
        {
            if(buffer[i] == '\n')
            {
                if(i > 0 && buffer[i - 1] == '\r')
                    buffer[i - 1] = 0;

                ProcessChatWithMachinState(buffer);
                if(need_to_delete)
                    break;

                int rest = buf_used - i - 1;
                memmove(buffer, buffer + i + 1, rest);
                buf_used = rest;
                break;
            }
        }
    }

}

void ClientSession::ProcessChatWithMachinState(const char *str)
{
    switch(current_state)
    {
        case fsm_ClientState::fsm_NewConnected:
            if(const char *error = ValidateName(str))
            {
                Send(error);
            }
            else
            {
                WelcomAndEnteredMsgAndSetName(str);
                current_state = fsm_ClientState::fsm_Normal;
            }
            break;
        case fsm_ClientState::fsm_Normal:
            if(str[0] == '/')
                CommandProcessLine(str);
            else
            {
                char *msg = new char[strlen(name) + strlen(str) + 5];
                sprintf(msg, "<%s> %s\n", name, str);

                the_master->SendAll(msg, this);
                delete[] msg;
            }
            break;
        case fsm_ClientState::fsm_ChangeName:
            if(const char *error = ValidateName(str))
            {
                Send(error);
            }
            else
            {
                SetName(str);
                current_state = fsm_ClientState::fsm_Normal;
            }
            break;
    }
}

void ClientSession::CommandProcessLine(const char *str)
{
    int nl = strlen(name);
    int serv_com = strlen(server_commands);
    char *buf = 0;

    if(strcmp(str, "/help") == 0) 
    {
        size_t needed = serv_com + nl + sizeof(what_commands_are_there) + 5;
        buf = new char[needed];
        snprintf(buf, needed, "%s %s, %s\n", server_commands, name, what_commands_are_there);
        Send(buf);                         
    }
    else if(strcmp(str, "/users") == 0)
    {
        buf = the_master->GetNumberUsersOnline();
        Send(buf);
    }
    else if(strcmp(str, "/name_users") == 0)
    {
        buf = the_master->GetNameUsersOnline();
        Send(buf);
    }
    else if(strcmp(str, "/change_name") == 0)
    {
        buf = new char[serv_com + nl + sizeof(new_name_msg) + 5];
        sprintf(buf, "%s %s, %s\n", server_commands, name, new_name_msg); 
        Send(buf);
        current_state = fsm_ClientState::fsm_ChangeName; 
    }
    else if(strcmp(str, "/quit") == 0)
    {
        need_to_delete = true;
    }
    else
    {
        buf = new char[serv_com + nl + sizeof(unknow_command_msg) + 5];
        sprintf(buf, "%s %s, %s\n", server_commands, name, unknow_command_msg);
        Send(buf);
    }

    if(buf)
        delete[] buf;
}

void ClientSession::DisconnectedClient()
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
}

void ClientSession::WelcomAndEnteredMsgAndSetName(const char *str)
{
    SetName(str);
    size_t len = strlen(str);
    char *wmsg = new char[len + sizeof(welcom_msg) + 2];
    sprintf(wmsg, "%s%s\n", welcom_msg, name);
    Send(wmsg);
    delete[] wmsg;

    char *emsg = new char[len + sizeof(entered_msg) + 2];

    sprintf(emsg, "%s%s\n", name, entered_msg);
    the_master->SendAll(emsg, this);
    delete[] emsg;
}

void ClientSession::SetName(const char *str)
{
    if(!str)
        return;
    
    if(name)
        delete[] name;
    name = strdup(str);
}

const char *ClientSession::ValidateName(const char *str)
{
    size_t len = strlen(str);

    if(len > 10)
        return invalid_name_msg;

    if(const char *already_name = the_master->IsNameUnique(str))
        return already_name;

    size_t i;
    for(i = 0; i < len; i++)
        if((str[i] < '0' || (str[i] > '9' && str[i] < 'A') || (str[i] > 'Z' && str[i] < 'a') || str[i] > 'z'))  
            return invalid_name_msg;

    return 0;
}

char *ClientSession::strdup(const char *str)
{
    if(!str)
        return 0;

    char *result = new char[strlen(str) + 1];
    strcpy(result, str);
    return result;
}

// class ChatServer

ChatServer *ChatServer::Start(EventSelector *sel, int port, int worker_com_channel[WORKERS_COUNT][STREAMS_COUNT])
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
    return new ChatServer(sel, ls, worker_com_channel);
}

ChatServer::ChatServer(EventSelector *sel, int fd, int worker_channel[WORKERS_COUNT][STREAMS_COUNT]) 
    : FdHandler(fd, true)
    , the_selector(sel)
    , current_worker(0)
{
    int i, j;
    for(i = 0; i < WORKERS_COUNT; i++)
        for(j = 0; j < STREAMS_COUNT; j++)
        worker_com_channel[i][j] = worker_channel[i][j];

    the_selector->Add(this);    
}

ChatServer::~ChatServer()
{
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
    {
        perror("accept filed");
        return;
    }

    current_worker = (current_worker + 1) % 3; 
    int worker_socket = worker_com_channel[current_worker][SOCKET_PARENT];

    char dummy = '\0';
    struct iovec io = {
        .iov_base = &dummy,
        .iov_len = 1
    };

    char cmsg_buf[CMSG_SPACE(sizeof(int))];
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



// class WorkerServer

void WorkerServer::worker_func_main(int my_idx, int socket_channel[WORKERS_COUNT][STREAMS_COUNT])
{
    EventSelector *selector = new EventSelector;  
    WorkerServer *workserv = new WorkerServer(selector, my_idx, socket_channel);

    selector->Run();

    delete workserv;
}

WorkerServer::WorkerServer(EventSelector *sel, int idx, int socket_channel[WORKERS_COUNT][STREAMS_COUNT]) 
    : FdHandler(socket_channel[idx][SOCKET_CHILD], true)
    , the_selector(sel)
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
                perror("sendmsg failed");
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

