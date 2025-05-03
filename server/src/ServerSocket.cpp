// server/src/ServerSocket.cpp

#include "../include/ServerSocket.h"
#include <iostream>
#include <thread>

TCPServer::TCPServer(uint16_t port) : address(port), socket() {} 

TCPServer::TCPServer(uint16_t port, const char *ip) : address(port, ip), socket() {}

TCPServer::~TCPServer()
{
    Stop();
}

void TCPServer::Start()
{
    socket.Bind(address.GetAddr(), address.GetAddrLen());
    socket.Listen(5);

    std::cout << "Server started..." << std::endl;

    while(true)
    {

    }
}

void TCPServer::Stop()
{
    for(std::vector<Session *>::iterator it = sessions.begin(); it != sessions.end(); ++it)
    {
        delete *it;
    }
}
