// server/inlcude/Chat/chat.hpp

#ifndef CHAT_HPP
#define CHAT_HPP

#define STAR_VERSION_WHISOUT_PROCCES 0

#define WORKERS_COUNT 3
#define STREAMS_COUNT 2
#define SOCKET_PARENT 0
#define SOCKET_CHILD 1

#include "../Sockets/sockets.hpp"

static const char welcom_msg[] = "Welcom to the Chat! You are known as ";
static const char entered_msg[] = " has entered the chat";
static const char left_msg[] = " has left the chat";

static const char server_commands[] = "<server command>";
static const char server_shutdown[] = "server shutdown";

static const char new_name_msg[] = "enter a new name:";
static const char invalid_name_msg[] = "Invalid name. The name must be less than 10 characters long"
                                       " and contain only letters or numbers."
                                       "Try again:\n";
static const char name_already_take_msg[] = "Name already take. Choose another.\n";                                       

static const char what_commands_are_there[] =  "available commands: /help /users /name_users /change_name /quit /shutdown";
static const char unknow_command_msg[] = "unknow command. Write /help for commands list."; 


enum {
    max_line_length = 1023,
    qlen_for_listen = 16
};

enum class fsm_ClientState {
    fsm_NewConnected,
    fsm_Normal,
    fsm_ChangeName
};


class WorkerServer;
class ChatServer;

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

    void ReadAndIgnore();
    void ReadAndCheck();
    void CheckLines();
    void ProcessChatWithMachinState(const char *str);

    virtual void Handle(bool re, bool we);

    void WelcomAndEnteredMsgAndSetName(const char *str);
    void CommandProcessLine(const char *str);
    void SetName(const char *name);
    void DisconnectedClient();

    const char *ValidateName(const char *name);
    
    static char *strdup(const char *str);
};

class WorkerSession : FdHandler
{
    friend class ChatServer;
    char buffer[max_line_length + 1];
    ChatServer *the_master;

    WorkerSession(ChatServer *a_master, int fd);

    virtual void Handle(bool re, bool we);
};

class ChatServer : public FdHandler 
{
    EventSelector *the_selector;
    struct item
    {
        WorkerSession *s;
        item *next;
    };
    item *first;
    int current_worker; 

    ChatServer(EventSelector *sel, int fd, int worker_pipes[WORKERS_COUNT][STREAMS_COUNT]); 
public:
    ~ChatServer();

    static ChatServer *Start(EventSelector *sel, int port, int worker_pipes_channel[WORKERS_COUNT][STREAMS_COUNT]);
    void ShutDownAllServer();
private:
    virtual void Handle(bool re, bool we);
};

class WorkerServer : public FdHandler
{
    EventSelector *the_selector;
    struct item
    {
        ClientSession *s;
        item *next;
    };
    item *first;
    int my_index;
    int worker_com_channel[WORKERS_COUNT][STREAMS_COUNT];
public:
    WorkerServer(EventSelector *sel, int idx, int channel[WORKERS_COUNT][STREAMS_COUNT]);
    ~WorkerServer();

    static void worker_func_main(int my_idx, int socket_channel[WORKERS_COUNT][STREAMS_COUNT]);

    void RemoveSession(ClientSession *s);
    char *GetNumberUsersOnline();
    char *GetNameUsersOnline();
    const char *IsNameUnique(const char *str);
    void SendAll(const char *msg, ClientSession *except = 0);
    void SendAllinTheWorkerProcess(const char *msg, ClientSession *except = 0);
    void SendServer(const char *msg);
    void ShutDownAllServer();
private:
    virtual void Handle(bool r, bool w);
};

#endif // CHAT_HPP
