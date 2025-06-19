// server/include/Sesions/ClientSesion.hpp

#ifndef CLIENTSESSION_HPP
#define CLIENTSESSION_HPP

#include "../Sockets/sockets.hpp"
#include "../Processes/WorkerProcess.hpp"
#include "../ConstantsAndVaribles.hpp"

class WorkerServer;

enum class fsm_ClientState {
    fsm_UnknowProtocol,

    fsm_HttpHost,
    fsm_HttpUserAgent,
    fsm_HttpAccept,

    fsm_ChatProtocolNewConnected,
    fsm_ChatProtocolNormal,
    fsm_ChatProtocolChangeName
};

class ClientSession : FdHandler
{
    friend class WorkerServer;

    char buffer[max_line_length + 1];
    int buf_used;
    bool ignoring;
    char *name;
    WorkerServer *the_master;
    fsm_ClientState current_state;
    bool need_to_delete;
    bool need_to_shutdown;

    ClientSession(WorkerServer *a_master, int fd);
    ~ClientSession();

    void Send(const char *msg);
    void Send(int file_fd, off_t file_size);

    void ReadAndIgnore();
    void ReadAndCheck();
    void CheckLines();
    void ProcessChatWithMachinState(const char *str);

    virtual void Handle(bool re, bool we);

    void WelcomAndEnteredMsgAndSetName(const char *str);
    void CommandProcessLine(const char *str);
    void SetName(const char *name);

    void HandleHttp(const char *str);
    bool SENDFile(const char *path);
    void SENDErrorHTML();
    char *GetMimeType(const char *path);
    char *Headers(const char *mimetype, off_t filesize);
    void DisconnectedClient();
    const char *ValidateName(const char *name);
    static char *strdup(const char *str);
};

#endif // CLIENTSESSION_HPP
