// server/incldue/Processes/ServerProcess.hpp

#ifndef SERVERPROCESSES_HPP
#define SERVERPROCESSES_HPP

#include "../Sockets/sockets.hpp"
#include "../Sessions/WorkerSession.hpp"
#include "../ConstantsAndVaribles.hpp"

class WorkerSession;

class Server : public FdHandler 
{
    EventSelector *the_selector;
    struct item
    {
        WorkerSession *s;
        item *next;
    };
    item *first;
    int current_worker; 

    Server(EventSelector *sel, int fd, int worker_pipes[WORKERS_COUNT][STREAMS_COUNT]); 
public:
    ~Server();
    static Server *Start(EventSelector *sel, int port, int worker_pipes_channel[WORKERS_COUNT][STREAMS_COUNT]);
    void ShutDownAllServer();
private:
    virtual void Handle(bool re, bool we);
};

#endif // SERVERPROCESSES_HPP
