// server/include/Socket.h
#ifndef SOCKET_H
#define SOCKET_H 

#include <sys/socket.h>

// Виртуальный класс который от которого наследуются сокет на TCP или сокет на  UDP 

class Socket
{
protected:
    int socket_fd;
public:
    Socket(int domain, int type, int protocol);
    virtual ~Socket();

    virtual void Bind(const struct sockaddr* addr, socklen_t addrlen) = 0;
    virtual void Listen(int backlog) = 0;
    // Другие общие методы ,,,
};


#endif // SOCKET_H
