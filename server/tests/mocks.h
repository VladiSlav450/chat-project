// tests/mocks.h

#ifndef MOCK_H
#define MOCK_H

#include <sys/socket.h>
#include <cstddef>

extern int (*original_bind)(int, const struct sockaddr *, socklen_t addrlen);
extern int (*original_listen)(int, int);

int mock_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int mock_listen(int sockfd, int backlog);

#endif // MOCK_H
