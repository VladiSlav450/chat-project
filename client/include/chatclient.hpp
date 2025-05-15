// client/include/chatclient.hpp

#ifndef CHATCLIENT_HPP
#define CHATCLIENT_HPP


#include "sockets.hpp"
#include "messageprocessor.hpp"
#include "guiinterface.hpp"

enum { max_line_length = 1023 };

class ChatClient;

class FLTKguiSession : public FdHandler
{
    friend ChatClient;

    FLTKguiSession(int fd);
    /* поля для обработки gui */

    char buffer[max_line_length + 1];
    ChatClient *the_master;

    virtual void Handle(bool re, bool we);
}; 

class ChatClient : public FdHandler
{
    EventSelector *the_selector;
    FLTKguiSession *fltk_sessoin;

    ChatClient(EventSelector *sel, int fd);
public:
    ~ChatClient();

    void Read();
    void Send(const char *msg);

    static ChatClient *Start(EventSelector *sel, const char *ip, const char *port);
private:
    virtual void Handle(bool we, bool re);
    void ValiPort(const char *port);
    bool ValidIp(const char *ip);
};

#endif // CHATCLIENT_HPP
