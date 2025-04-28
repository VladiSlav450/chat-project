// tests/mocks.h
#ifndef MOCK_H
#define MOCK_H
#include <sus/socket.h>

extern int (*original_bind)(int, const struct sockaddr *socklen_t addrlen);
extern int (*original_listen)(int, int);

int mock_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return (addr != NULL && addrlen > 0) ? 0 : -1;
}

int mock_listen(int sockfd, int backlog)
{
    return (sockfd > 0) ? 0 : -1;
}

#endif // MOCK_H
