// server/src/Session.cpp

#include "../include/Session.h"
#include <iostream>
#include <thread>

Session::Session(Client *c1, Client *c2) : client1(c1), client2(c2) {}

Session::~Session()
{
    delete client1;
    delete client2;
}
 
void Session::Run()
{

} 
