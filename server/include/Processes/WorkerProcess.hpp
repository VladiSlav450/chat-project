// server/include/Processes/WorkerProcesses.hpp

#ifndef WORKERPROCESS_HPP
#define WORKERPROCESS_HPP

#include "../Sockets/sockets.hpp"
#include "../Sessions/ClientSession.hpp"
#include "../ConstantsAndVaribles.hpp"

class ClientSession;
class RouteConfigFile;

class WorkerServer : public FdHandler
{
    EventSelector *the_selector;
    RouteConfigFile *configfile;

    struct item
    {
        ClientSession *s;
        item *next;
    };
    item *first;
    int my_index;
    int worker_com_channel[WORKERS_COUNT][STREAMS_COUNT];
public:
    WorkerServer(EventSelector *sel, int idx, int channel[WORKERS_COUNT][STREAMS_COUNT], RouteConfigFile *config);
    ~WorkerServer();

    static void worker_func_main(int my_idx, int socket_channel[WORKERS_COUNT][STREAMS_COUNT], const char *path_config);

    void RemoveSession(ClientSession *s);
    char *GetNumberUsersOnline();
    char *GetNameUsersOnline();
    const char *IsNameUnique(const char *str);
    void SendAll(const char *msg, ClientSession *except = 0);
    void SendAllinTheWorkerProcess(const char *msg, ClientSession *except = 0);
    void SendServer(const char *msg);
    void ShutDownAllServer();
    char *FindPathForSendFile(const char *path);
private:
    virtual void Handle(bool r, bool w);
};

class RouteConfigFile
{
    SparseArray<MyStr, MyStr> routes;
    const char *config_path;
    MyStr standart_key_404path;
    time_t last_mod_time;
public:
    RouteConfigFile(const char *path);
    ~RouteConfigFile();
    void ReloadConfig();
    char *SerchPath(const char *request_path);
};

#endif // WORKERPROCESS_HPP
