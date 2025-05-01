// server/src/Socket.cpp

#include "../include/Socket.h"
#include "../include/ExceptionError/ExternalError/ExternalError.h"
#include <unistd.h>

Socket::Socket(int domain, int type, int protocol)
{
    socket_fd = ::socket(domain, type, protocol); 
    if(socket_fd < 0)
        throw ExternalError("Failed create socket!", 101);  
}

Socket::~Socket()
{
    Close();
}

void Socket::Close()
{
    if(socket_fd >= 0)
        ::close(socket_fd);
}
