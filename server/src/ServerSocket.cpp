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
        struct sockaddr_in client1_addr, client2_addr;
        socklen_t len = sizeof(client1_addr);

        int fd1 = socket.Accept((struct sockaddr *)&client1_addr, &len);
        std::cout << "Client 1 connected!" << std::endl;

        int fd2 = socket.Accept((struct sockaddr *)&client2_addr, &len);
        std::cout << "Client 2 connected!" << std::endl;

        Client *c1 = new Client(fd1, client1_addr);
        Client *c2 = new Client(fd2, client2_addr);
        
        Session *session = new Session(c1, c2);
        sessions.push_back(session);
        session->Run(); 
    }
}

void TCPServer::Stop()
{
    for(std::vector<Session *>::iterator it = sessions.begin(); it != sessions.end(); ++it)
    {
        delete *it;
    }
}
