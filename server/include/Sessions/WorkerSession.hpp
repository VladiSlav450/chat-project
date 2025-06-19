// server/include/Session/WorkerSession.hpp
  
#ifndef WORKERSESSION_HPP
#define WORKERSESSION_HPP

#include "../Processes/ServerProcess.hpp"
#include "../Sockets/sockets.hpp"
#include "../ConstantsAndVaribles.hpp"

class Server;

class WorkerSession : FdHandler
{
    friend class Server;
    char buffer[max_line_length + 1];
    Server *the_master;

    WorkerSession(Server *a_master, int fd);
    virtual void Handle(bool re, bool we);
};


#endif // WORKERSESSION_HPP



