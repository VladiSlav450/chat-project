// server/include/Socket.h

#ifndef SOCKET_H
#define SOCKET_H 

#include <sys/socket.h>

// Общий базовый класс сокетов 

class Socket
{
protected:
    int socket_fd;
public:
    Socket(int domain, int type, int protocol);
    virtual ~Socket();

    void GetSocket() const { return socket_fd; }
    void Close();
};

#endif // SOCKET_H
