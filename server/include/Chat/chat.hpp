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

class ChatServer;

class ChatSession : FdHandler
{
    friend class ChatServer;

    char buffer[max_line_length + 1];
    int buf_used;
    bool ignoring;
    char *name;
    ChatServer *the_master;

    fsm_ClientState current_state;
    bool need_to_delete;

    ChatSession(ChatServer *a_master, int fd);
    ~ChatSession();

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

class ChatServer : public FdHandler 
{
    EventSelector *the_selector;
    struct item 
    {
        ChatSession *s;
        item *next; 
    };
    item *first;

    ChatServer(EventSelector *sel, int fd); 
    
public:
    ~ChatServer();

    static ChatServer *Start(EventSelector *sel, int port);

    void RemoveSession(ChatSession *s);

    void SendAll(const char *msg, ChatSession *except = 0);

    char *GetNumberUsersOnline();
    char *GetNameUsersOnline();
    const char *IsNameUnique(const char *str);
private:
    virtual void Handle(bool re, bool we);
};

#endif // CHAT_HPP
