// tests/mocks.cpp
#include "mocks.h"

int (*original_bind)(int, const struct sockaddr *socklen_t addrlen) = bind;
int (*original_listen)(int, int) = listen;
