// server/inlcude/Chat/chat.hpp

#ifndef CHAT_HPP
#define CHAT_HPP

#include "../Sockets/sockets.hpp"

static const char welcom_msg[] = "Welcom to the Chat! You are known as ";
static const char entered_msg[] = " has entered the chat";
static const char left_msg[] = " has left the chat";

static const char invalid_name_msg[] = "Invalid name. Try again:\n";
static const char what_commands_are_there[] =  "Commands: /help /users /name_users /change_name /quit";
static const char unknow_command[] = "Unknow command. Write /help for commands list."; 


enum {
    max_line_length = 1023,
    qlen_for_listen = 16
};

enum class fsm_ClientState {
    fsm_NewConnected;
    fsm_Normal;
    fsm_ChangeName;
    fsm_Error;
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

    ChatSession(ChatServer *a_master, int fd);
    ~ChatSession();

    void Send(const char *msg);

    void ReadAndIgnore();
    void ReadAndCheck();
    void CheckLines();
    void ProcessWithMachinState();
    void ProcessLine(const char *str);

    virtual void Handle(bool re, bool we);

    void CommandProcessLine(const char *str);
    void SetName(const char *buffer);

    bool ValidateName(const char *buffer);

    char *Command_Help();
    char *Command_Number_Users_Online();
    char *Command_Name_Users_Online();
    char *Command_Change_Name();
    char *Command_Quit();
    char *Command_Unkown_Command();
    
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
    char *Get_Name_Online_User();
private:
    virtual void Handle(bool re, bool we);
};

#endif // CHAT_HPP
