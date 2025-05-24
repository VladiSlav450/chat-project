// server/inlcude/Chat/chat.hpp

#ifndef CHAT_HPP
#define CHAT_HPP

#include "../Sockets/sockets.hpp"

static const char welcom_msg[] = "Welcom to the Chat! You are known as ";
static const char entered_msg[] = " has entered the chat";
static const char left_msg[] = " has left the chat";

static const char what_commands_are_there[] =  "Commands: /help /users /name_users /change_name /quit";
static const char unknow_command[] = "Unknow command. Write /help for commands list."; 


enum {
    max_line_length = 1023,
    qlen_for_listen = 16
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

    ChatSession(ChatServer *a_master, int fd);
    ~ChatSession();

    void Send(const char *msg);

    void ReadAndIgnore();
    void ReadAndCheck();

    void CheckLines();
    void ProcessLine(const char *str);

    virtual void Handle(bool re, bool we);

    char *CommandProcessLine(const char *str);
    char *Command_Help();
    char *Command_Number_Users_Online();
    char *Command_Name_Users_Online();
    char *Command_Change_Name();
    char *Command_Quit();
    char *Command_Unkown_Command();
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
    char *GetOnlineUser();
private:
    virtual void Handle(bool re, bool we);
};

#endif // CHAT_HPP
