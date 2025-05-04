// server/inlcude/Chat/chat.hpp

#ifndef CHAT_HPP
#define CHAT_HPP

#include "../Socket/socket.hpp"

enum {
    max_line_length = 1023,
    qlen_for_listen = 16
};

class ChatSession : FdHandler
{
    friend class ChatServer;
    char buffer[max_line_length + 1];
    int buf_used;
    bool ignoring;
    char *name;
    ChatServer *the_master;

    ChatSession(ChatServer *a_master, in_fd);
    ~ChatSession();

    void Send(const chat *msg);
    virtual void Handle(bool re, bool we);
    void ReadAndIgnore();
    void RadAndCheck();
    void CheckLines();
    void ProcessLine(const char *str);
};

class ChatServer : public FdHandler 
{
    EventSelector *the_selector;
    strucu item 
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
    void SandAll(const char *msg, ChatSession *except = 0);

private:
    virtual void Handle(bool re, bool we);
};

#endif // CHAT_HPP
