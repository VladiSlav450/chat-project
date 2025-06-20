// server/src/Sessions/ClientSession.cpp

#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <string.h>


#include "../../include/Sessions/ClientSession.hpp"

// class ClientSession

ClientSession::ClientSession(WorkerServer *a_master, int fd) 
    : FdHandler(fd, true)
    , buf_used(0)
    , ignoring(false)
    , name(0)
    , the_master(a_master)
    , current_state(fsm_ClientState::fsm_UnknowProtocol)
    , need_to_delete(false)
    , need_to_shutdown(false)   {}

ClientSession::~ClientSession()
{
    if(name)
        delete[] name;
}

void ClientSession::Send(const char *msg)
{
    ssize_t res;
    res = write(GetFd(), msg, strlen(msg));    
    if(res < 0)
        perror("Send write failed ");
}

void ClientSession::Send(int file_fd, off_t file_size)
{
    off_t offset = 0;
    ssize_t sent;
    int opt = 1;
    int fd = GetFd();

    if(setsockopt(fd, SOL_TCP, TCP_CORK, &opt, sizeof(opt)) < 0) 
        perror("Setsockopt TCP_CORK send file: ");

    while(file_size > 0)
    {
        sent = sendfile(fd, file_fd, &offset, file_size);
        if(sent <= 0)
        {
            perror("sendfile error");
            break;
        }
        file_size -= sent;
    } 

    opt = 0;
    setsockopt(fd, SOL_TCP, TCP_CORK, &opt, sizeof(opt));
    fsync(fd);
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
    else if(need_to_shutdown)
        the_master->ShutDownAllServer();
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
    while(buf_used > 0 && !need_to_delete && !need_to_shutdown) 
    {
        int i;
        for(i = 0; i < buf_used; i++)
        {
            if(buffer[i] == '\n')
            {
                if(i > 0 && buffer[i - 1] == '\r')
                    buffer[i - 1] = 0;

                ProcessChatWithMachinState(buffer);
                if(need_to_delete || need_to_shutdown)
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
        case fsm_ClientState::fsm_UnknowProtocol:
            if(strcmp(str, "CHAT_PROTOCOL") == 0)
            {
                Send("Your name please: "); 
                current_state = fsm_ClientState::fsm_ChatProtocolNewConnected;
            }
            else if(strncmp(str, "GET /", 5) == 0)
            {
                printf("%s\n", str);
                HandleHttp(str + 4);
                current_state = fsm_ClientState::fsm_HttpHost;
            }
            break;
        case fsm_ClientState::fsm_HttpHost:
            printf("%s\n", str); 
            current_state = fsm_ClientState::fsm_HttpUserAgent;
            break;
        case fsm_ClientState::fsm_HttpUserAgent:
            printf("%s\n", str); 
            current_state = fsm_ClientState::fsm_HttpAccept;
            break;
        case fsm_ClientState::fsm_HttpAccept:
            printf("%s\n---------------------\n", str); 
            need_to_delete = true;
            break;
        case fsm_ClientState::fsm_ChatProtocolNewConnected:
            if(const char *error = ValidateName(str))
                Send(error);
            else
            {
                WelcomAndEnteredMsgAndSetName(str);
                current_state = fsm_ClientState::fsm_ChatProtocolNormal;
            }
            break;
        case fsm_ClientState::fsm_ChatProtocolNormal:
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
        case fsm_ClientState::fsm_ChatProtocolChangeName:
            if(const char *error = ValidateName(str))
                Send(error);
            else
            {
                SetName(str);
                current_state = fsm_ClientState::fsm_ChatProtocolNormal;
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
        current_state = fsm_ClientState::fsm_ChatProtocolChangeName; 
    }
    else if(strcmp(str, "/quit") == 0)
    {
        need_to_delete = true;
    }
    else if(strcmp(str, "/shutdown") == 0)
    {
        need_to_shutdown = true;
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

/*
   1) addr: / - index.html
   2) addr: /'любая_хуйняэ' - 404.html
*/
void ClientSession::HandleHttp(const char *str)
{
    char path[max_line_length + 1];
    int i;
    for(i = 0; str[i] != ' '; i++)
        path[i] = str[i];
    path[i] = 0;

    char *file_path = the_master->FindPathForSendFile(path);
    if(file_path)
    {
        if(!SENDFile(file_path))
            SENDErrorHTML();
        free(file_path);
    }
    else
    {
        printf("Path file filed ClientSession\n");
        SENDErrorHTML();
    }
}

bool ClientSession::SENDFile(const char *path)
{
    int file_fd = open(path, O_RDONLY);
    if(file_fd < 0)
    {
        printf("\nFile open error (%s): %s\n", path, strerror(errno));
        return false;
    }
    struct stat file_stat;
    fstat(file_fd, &file_stat);

    char *mime_type = GetMimeType(path);
    char *head = Headers(mime_type, file_stat.st_size);
    Send(head);

    delete[] mime_type;
    delete[] head;

    Send(file_fd, file_stat.st_size);
    close(file_fd);
    return true;
}

char *ClientSession::GetMimeType(const char *path)
{
    ssize_t i;
    size_t dot_pos = 0;
    char *mimetype;
    for(i = strlen(path) - 1; i > 0; i--)
    {
        if(path[i] == '.')
        {
            dot_pos = i;
            break;
        }
    }
    printf("Path = %s\n", path);
    if(strcmp(path + dot_pos, ".html") == 0)
    {
        const char txthtml[] = "text/html"; 
        mimetype = new char[strlen(txthtml) + 1];
        strcpy(mimetype, txthtml); 
    }
    else if(strcmp(path + dot_pos, ".css") == 0)
    {
        const char txtcss[] = "text/css"; 
        mimetype = new char[strlen(txtcss) + 1];
        strcpy(mimetype, txtcss); 
    }
    else if(strcmp(path + dot_pos, ".jpg") == 0)
    {
        const char imgjpg[] = "image/jpeg"; 
        mimetype = new char[strlen(imgjpg) + 1];
        strcpy(mimetype, imgjpg); 
    }
    else if(strcmp(path + dot_pos, ".gif") == 0)
    {
        const char imggif[] = "image/gif"; 
        mimetype = new char[strlen(imggif) + 1];
        strcpy(mimetype, imggif); 
    } 
    else
    {
        const char appocst[] = "aplication/octet-stream"; 
        mimetype = new char[strlen(appocst) + 1];
        strcpy(mimetype, appocst); 
    }
    
    return mimetype;
}

char *ClientSession::Headers(const char *mimetype, off_t filesize)
{
    off_t count = 0;
    off_t tmp = filesize;
    do
    {
        tmp = tmp / 10; 
        count++;
    }
    while(tmp);
    const char begin[] = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: ";
    const char middle[] = "Content-Length: ";
    const char end[] = "Connection: close\r\n"
                       "\r\n"; 
    char *response = new char[strlen(begin) + strlen(middle) + strlen(mimetype) + strlen(end) + count + 5];
    sprintf(response, "%s%s\r\n%s%ld\r\n%s", begin, mimetype, middle, filesize, end);
    return response;
}

void ClientSession::SENDErrorHTML()
{
    Send("HTTP/1.1 404 Not Found\r\n"
         "Content-Type: text/plain\r\n"
         "\r\n"
         "404 Not Found");
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
