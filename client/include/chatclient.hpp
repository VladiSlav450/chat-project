// client/include/chatclient.hpp

#ifndef CHATCLIENT_HPP
#define CHATCLIENT_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>

#include <X11/Xlib.h>
  

#include "sockets.hpp"


class ChatClient;

enum {  
    WIDTH_WINDOW = 600,
    HEIGTH_WINDOW = 400,

    MARGIN_SPACING = 20,
    CHAT_DISPLAY_H = 360,
    INPUT_FIELD_W = 460,
    INPUT_FIELD_H = 30,
    BUTTON_W = 90
};

class FLTKguiSession : FdHandler
{

    friend ChatClient;

    ChatClient *the_master;

    F1_Window *window;

    F1_Text_Buffer *chat_buffer;
    F1_Text_Display *chat_display;

    F1_Input *input_field;
    F1_Button *send_button;

    FLTKguiSession(int fd);
    ~FLTKguiSession();

    static FLTKguiSession *Start(int fd);

    void SendCallback(F1_Widget *w, void *data);
    void Show(const char *msg);
    virtual void Handle(bool re, bool we);

}; 

class ChatClient : public FdHandler
{
    EventSelector *the_selector;
    FLTKguiSession *fltk_session;

    ChatClient(EventSelector *sel, FLTKguiSession *disp, int fd);
public:
    ~ChatClient();

    void Send(const char *msg);

    static ChatClient *Start(EventSelector *sel, const char *ip, const char *port);
private:
    virtual void Handle(bool re, bool we);

    void ValiPort(const char *port);
    bool ValidIp(const char *ip);
};

#endif // CHATCLIENT_HPP
