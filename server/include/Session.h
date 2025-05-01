// server/include/Session.h

#ifndef SESSION_H
#define SESSION_H

#include "Client.h"
#include <vector>

class Session
{
    Client *client1;
    Client *client2;
public:
    Session(Client *c1, Client *c2);
    ~Session();

    void Run();
};

#endif // SESSION_H

