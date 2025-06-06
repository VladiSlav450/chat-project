// server/inlcude/Chat/chat.hpp

#ifndef CHAT_HPP
#define CHAT_HPP

#include "../Sockets/sockets.hpp"

static const char welcom_msg[] = "Welcom to the Chat! You are known as ";
static const char entered_msg[] = " has entered the chat";
static const char left_msg[] = " has left the chat";

static const char server_commands[] = "<server command>";

static const char new_name_msg[] = "enter a new name:";
static const char invalid_name_msg[] = "Invalid name. The name must be less than 10 characters long"
                                       " and contain only letters or numbers."
                                       "Try again:\n";
static const char name_already_take_msg[] = "Name already take. Choose another.\n";                                       

static const char what_commands_are_there[] =  "available commands: /help /users /name_users /change_name /quit";
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


class ChatServer : public FdHandler 
{
    EventSelector *the_selector;
    int **worker_pipes_channel;
    int next_worker;

    ChatServer(EventSelector *sel, int fd, int **worker_pipes_channel); 
    
public:
    ~ChatServer();

    static ChatServer *Start(EventSelector *sel, int port, int **worker_pipes_channel);

private:
    virtual void Handle(bool re, bool we);
};



class ClientSession : FdHandler
{
    
    friend class WorkerServer;

    char buffer[max_line_length + 1];
    int buf_used;
    bool ignoring;
    char *name;
    ChatServer *the_master;

    fsm_ClientState current_state;
    bool need_to_delete;

    ClientSession(ChatServer *a_master, int fd);
    ~ClientSession();

    void Send(const char *msg);

    void ReadAndIgnore();
    void ReadAndCheck();
    void CheckLines();
    void ProcessChatWithMachinState(const char *str);

//    void ProcessLine(const char *str);

    virtual void Handle(bool re, bool we);

    void WelcomAndEnteredMsgAndSetName(const char *str);
    void CommandProcessLine(const char *str);
    void SetName(const char *name);
    void DisconnectedClient();

    const char *ValidateName(const char *name);
    
    static char *strdup(const char *str);
};

class WorkerServer : public FdHandler
{
    EventSelector *the_selector;
    struct item
    {
        ClientSession *s;
        item *next;
    }
    item *first;
public:
    WorkerServer(EventSelector *sel, int fd);
    ~WorkerServer();

    void RemoveSession(ClientSession *s);

/*  Тут ещё должны быть мктоды 
 *  для отправки количестов в онлайне, кто они,
 *  void SendAll(const char *msg, ClinetSession *except);
 */

private:
    virtual void Handle(bool r, bool w);
}

#endif // CHAT_HPP
