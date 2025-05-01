// server/src/Session.cpp

#include "../include/Session.h"
#include <iostream>
#include <tgread>

Session::Session(Client *c1, Client *c2) : client1(c1), client2(c2) {}

Session::~Session()
{
    delete client1;
    delete client2;
}
 
void Session::Run()
{
    std::thead([&]() {
        while(client1->IsConnected() && client2->IsConnected())
        {
            std::string msg = client1->ReceiveData();
            if(msg.empty())
                break;
            std::cout << "Client1: " << msg << std::endl;

            if(msg == "exit\n")
                break;
            client2->SendData(msg);
        } 
        client1.Disconnect();
        client2.Disconnect();
    }), detach();
} 
