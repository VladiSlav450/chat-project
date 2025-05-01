// tests/mocks.cpp

#include "mocks.h"

int (*original_bind)(int, const struct sockaddr *, socklen_t addrlen) = bind;
int (*original_listen)(int, int) = listen;

int mock_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return (addr != NULL && addrlen > 0) ? 0 : -1;
}

int mock_listen(int sockfd, int backlog)
{
    return (sockfd > 0) ? 0 : -1;
}
