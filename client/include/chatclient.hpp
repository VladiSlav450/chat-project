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

enum {  
    max_line_length = 1023,

    SPACING = 20,

    SIZEFONT = 14,

    LABEL_W = 50,

    BUTTON_W = 100,
    BUTTON_H = 40,

    DISPLAY_W = BUTTON_W * 3 + SPACING * 2,
    DISPLAY_H = BUTTON_W * 3,

    INPUT_W = BUTTON_W * 3 + 2 * SPACING - LABEL_W,
    INPUT_H = 30,

    WINDOW_W = DISPLAY_W + SPACING * 2,
    WINDOW_H = DISPLAY_H + INPUT_H + BUTTON_H + SPACING * 4
};

class ChatClient;

class FLTKguiSession : FdHandler
{
    friend ChatClient;

    ChatClient *the_master;

    Fl_Window *window;
    Fl_Text_Buffer *chat_buffer;
    Fl_Text_Display *display;
    Fl_Input *input;
    Fl_Button *send_button;

    FLTKguiSession(int fd, Fl_Window *win, Fl_Text_Buffer *ch_buf, Fl_Text_Display *chat_disp, Fl_Input *inp, Fl_Button *send_but);
    ~FLTKguiSession();

    static FLTKguiSession *Start(Display *display);
    static void SendCallback(Fl_Widget *w, void *data);

    void PrintAndSend();
    void Show(const char *msg);
    virtual void Handle(bool re, bool we);

}; 

class ChatClient : public FdHandler
{
    friend FLTKguiSession;

    EventSelector *the_selector;
    FLTKguiSession *fltk_session;

    ChatClient(EventSelector *sel, FLTKguiSession *disp, int fd);
public:
    ~ChatClient();

    void Send(const char *msg);

    static ChatClient *Start(EventSelector *sel, const char *ip, const char *port);
private:
    virtual void Handle(bool re, bool we);

    static int ValidPort(const char *port);
    static void ValidIp(const char *ip);
};

#endif // CHATCLIENT_HPP
