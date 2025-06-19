// server/src/Sessions/WorkerSession.cpp

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/uio.h>
#include <sys/socket.h>


#include "../../include/Sessions/WorkerSession.hpp"

// class WorkerSession

WorkerSession::WorkerSession(Server *a_master, int fd) 
    : FdHandler(fd, true)
    , the_master(a_master) 
{}

void WorkerSession::Handle(bool re, bool we)
{
    if(!re)
        return;

    struct iovec io = {
        .iov_base = buffer,
        .iov_len = sizeof(buffer) - 1
    };

    char cmsg_buf[CMSG_SPACE(sizeof(int))];

    struct msghdr msg = {0};
    msg.msg_iov = &io;
    msg.msg_iovlen = 1; 
    msg.msg_control = cmsg_buf;
    msg.msg_controllen = sizeof(cmsg_buf);


    int res = recvmsg(GetFd(), &msg, MSG_DONTWAIT);
    if(res < 0)
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
            perror("recvmsg WorkerSession::Handle failed");
        return; 
    }

    if(res > 0)
    {
        // значит это сообщение о выключении сервера
        buffer[res] = '\0'; 
        char *strshutdown = new char[strlen(server_commands) + strlen(server_shutdown) + 3];
        sprintf(strshutdown, "%s %s\n", server_commands, server_shutdown);
        if(strncmp(buffer, strshutdown, strlen(strshutdown)) == 0)
            the_master->ShutDownAllServer();
        else
            printf("command /shutdown message in WorkerSession failed\n");
        delete[] strshutdown;
    }
}

