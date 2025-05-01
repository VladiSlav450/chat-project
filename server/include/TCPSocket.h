// server/include/TCPSocket.h

#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "Socket.h"

class TCPSocket : public Socket
{
public:
    TCPSocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);

    void Bind(const struct sockaddr *addr, socklen_t addrlen);
    void Listen(int backlog);
    int Accept(struct sockaddr *addr, socklen_t *addrlen);
};

#endif // TCPSOCKET_H
