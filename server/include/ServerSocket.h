// server/include/ServerSocket.h

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "TCPSocket.h"
#include "IPv4Address.h"
#include "Session.h"
#include <vector>

class TCPServer
{
    TCPSocket socket;
    IPv4Address address;
    std::vector<Session *> sessions;
public:
    TCPServer(uint16_t port);
    TCPServer(uint16_t port, const char *ip);
    ~TCPServer();
    
    void Start();
    void Stop();
};

#endif // SERVERSOCKET_H
