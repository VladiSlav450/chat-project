// server/include/ServerSocket.h
#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "Socket.h"
#include "IPv4Address.h"

class TCPServer
{
    TCPSocket socket;
    IPv4Address address;
    vector<Client> clients;
public:
    TCPServer(uint16_t port) : address(port), socket();
    void Start();
    void Stop();
};

#endif // SERVERSOCKET_H
