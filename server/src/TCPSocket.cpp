// server/src/TCPSocket.cpp

#include "../include/TCPSocket.h"
#include "../include/ExceptionError/ExternalError/ExternalError.h"
#include <sys/socket.h>

TCPSocket::TCPSocket(int domain = AF_INET, int type = SOCK_STREAM, int protocol = 0) : Socket(domain, type, protocol) {}

void TCPSocket::Bind(const struct sockaddr* addr, socklen_t addrlen)
{
    if(::bind(socket_fd, addr, socklen_t) < 0)
        throw ExternalError("Failed to bind Socketa at function Bind!", 102);
}

void TCPSocket::Listen(int backlog)
{
    if(::listen(socket_fd, backlog) < 0)
            throw ExternalError("Failed to listen on socket at function Listen!", 103);
}

int TCPSocket::Accept(struct sockaddr *addr, socklen_t addrlen)
{
    int client_fd = ::accept(socket_fd, addr, addrlen);
    if(clietn_fd < 0)
        throw ExternalError("Failed to accept connection", 104);
    return client_fd;
}
