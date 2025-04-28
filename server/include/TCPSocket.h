// server/include/TCPSocket.h
#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "Socket.h"

class TCPSocket : public Socket
{
public:
    TCPSocket() : Socket(AF_INET, SOCK_STREAM, 0) {}
    virtual void Bind(const struct sockaddr* addr, socklen_t addrlen);
    virtual void Listen(int backlog);
};

#endif // TCPSOCKET_H
