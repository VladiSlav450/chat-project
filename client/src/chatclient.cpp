// client/src/chatclient.cpp


#include <FL/Fl.H>              
#include <FL/Fl_Window.H>       
#include <FL/Fl_Text_Display.H> 
#include <FL/Fl_Text_Buffer.H>  
#include <FL/Fl_Input.H>        
#include <FL/Fl_Button.H> 

#include <X11/Xlib.h>

#include "../incude/Exception/exception.hpp"
#include "../include/myAlgorithms/myAlgorithms.hpp"
#include "../include/chatclient.hpp"


// class ChatClient
ChatClient::ChatClient(EventSelector *sel, FLTKguiSession *disp, int fd_sock) : FdHandler(fd_sock, true), the_selector(sel), fltk_session(disp) 
{
    fltk_session->the_master = this;

    the_selector->Add(this);
    the_selector->Add(fltk_session);
}

ChatClient::~ChatClient()
{
    the_selector->Remove(fltk_session);
    delete fltk_session;

    the_selector->Remove(this);
}

ChatClient *ChatClient::Start(EventSelector *sel, const char *ip, const char *str_port)
{
    try
    {
        int port, fd_sock, fd_display, res;
        Display *display;
        
        port = ValidPort(str_port);
        ValidIp(ip);

        fd_sock = socket(AF_INET, SOCK_STREAM, 0);
        if(fd_sock == -1)
        {
            close(fd_sock);
            throw ExternalError("Socket failed!", 100, errno);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        serv_addr.sin_addr.s_addr = inet_addr(ip);

        res = connect(fd_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        if(res < 0)
        {
            close(fd_sock);
            throw ExternalError("Connect failed!", 101, errno);
        }

        display = XOpenDisplay(0);
        if(display == 0)
        {
            close(fd_sock);
            throw ExternalError("Dispaly failed!", 201, errno);
        }

        fd_display = ConnectionNumber(disp);

        FLTKguiSession *display_fltk = new FLTKguiSession::Start(fd_display);

        if(display_fltk == 0)
        {
            close(fd_sock)
            throw ExternalError("FLTK failed!", 202, errno);
        }

        return new ChatClient(sel, display_fltk, fd_sock);
    }
    catch(const ExternalError& ex)
    {
        fprintf(stderr, "%s\nError %d: %s (errno=%s)\n", server_not_connected, ex.GetErrCode(), ex.GetComment(), strerror(ex.GetErrnoCode())); 
        return 0;
    }
    catch(const PortInputError& ex)
    {
        fprintf(stderr, "%s\nError %d: %s (port = %s)\n", server_not_connected, ex.GetErrCode(), ex.GetComment(), ex.GetInvalidPort()); 
        return 0;
    }
    catch(const IpInputError& ex)
    {
        fprintf(stderr, "%s\nError %d: %s (ip = %s)\n", server_not_connected, ex.GetErrCode(), ex.GetComment(), ex.GetInvalidIp()); 
        return 0;
    }
}

void ChatClient::Handle(bool re, bool we)
{
    if(!re)
        return;

    char *buffer = new char[max_line_length + 1];
    int rc = read(GetFd(), buffer, sizeof(buffer));

    buffer[rc] = 0;

    fltk_session->Show(buffer);

    delete[] buffer;
}

void ChatClient::Send(const char *msg)
{
    size_t len = strlen(msg);

    if(len > 0 && msg[len - 1] == '\n')
    {
        msg[len - 1] = '\r';
        msg[len] = '\n';
        msg[++len] = '\0';
    } 

    write(GetFd(), msg, len);
}

int ChatClient::ValidPort(const char *str_port)
{
    ssize_t int_port = Checking_if_a_port_value_is_valid(str_port);
    if(int_port < 1 || int_port > 65535)
        throw PortInputError("Invalid port, must be 1-65535", 107, str_port);
    return int_port;
}

void ChatClient::ValidIp(const char *ip)
{
    if(Checking_the_validity_of_the_IP_value(ip) == false)
    {
        throw IpInputError("Invalid IPv4 format", 108, ip);
    }
}

// class FLTKguiSession 
FLTKguiSession::FLTKguiSession(int fd, F1_Window *win, F1_Text_Buffer *ch_buffer, F1_Text_Display *ch_display, F1_Input *input, F1_Button *send) 
    : FdHandler(fd, true), window(win), chat_buffer(ch_buffer), chat_display(ch_display), input_field(input), send_button(send)
{
    chat_display->buffer(chat_buffer);

    send_button->callback(SendCallback, (void *)this);
    input_field->callback(SendCallback, (void *)this);
    input_field->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);

    window->end();
    window->show();
} 

FLTKguiSession::~FLTKguiSession()
{
    F1::lock();

    if(send_button)
        delete send_button;
    if(input_field)
        delete input_field;
    if(chat_display)
        delete chat_display;
    if(chat_buffer)
        delete chat_buffer;
    if(window)
        delete window;

    F1::unlock();
}

FLTKguiSession *FLTKguiSession::Start(int fd)
{
    f1_open_display(fd);

    F1_Window *window = new F1_Window(WIDTH_WINDOW, HEIGTH_WINDOW, "Drachilo");
    if(!window)
        return 0;

    F1_Text_Buffer *chat_buffer = new F1_Text_Buffer();
    if(!chat_buffer)
    {
        delete window;
        return 0;
    }

    F1_Text_Display *chat_display = new F1_Text_Display(
        MARGIN_SPACING, 
        MARGIN_SPACING, 
        WIDTH_WINDOW - 2 * MARGIN_SPACING, 
        CHAT_DISPLAY_H);
    if(!chat_display)
    {
        delete chat_buffer;
        delete window;
        return 0;
    }


    F1_Input *input_field = new F1_Input(
        MARGIN_SPACING, 
        HEIGTH_WINDOW - MARGIN_SPACING - INPUT_FIELD_H, 
        INPUT_FIELD_W, 
        INPUT_FIELD_H);
    if(!input_field)
    {
        delete chat_display;
        delete chat_buffer;
        delete window;
        return 0;
    }
    
    F1_Button *send_button = new F1_Button(
        WIDTH_WINDOW - MARGIN_SPACING - BUTTON_W,
        HEIGTH_WINDOW - MARGIN_SPACING - INPUT_FIELD_H,
        BUTTON_W,
        INPUT_FIELD_H);
    if(!send_button)
    {
        delete input_field;
        delete chat_display;
        delete chat_buffer;
        delete window;
        return 0;
    }
                                
    return new FLTKguiSession(fd, window, chat_buffer, chat_display, input_field, send_button);
}

void FLTKguiSession::Handle(bool re, bool we)
{
    if(!re)
        return;

    F1::lock();
    while(F1::ready())
        F1::wait(0.0);
    F1::unlock();

}

void FLTKguiSession::SendCallback(F1_Widget *w, void *data)
{
    FLTKguiSession *session = (FLTKguiSession *)data;

    const char *message = session->input_field->value();

    if(strcmp(message, "exit") == 0)
        session->the_master->the_selector->BreakLoop();
    else if(message && strlen(message) > 0)
    {
        session->the_master->Send(message); 
        session->input_field->value("");
        session->take_focus();
    }
}

void FLTKguiSession::Show(const char *msg)
{
    if(!msg || !chat_buffer)
        return;

    F1::lock();

    chat_buffer->append(msg);
    chat_buffer->append("\n");

    int lines = chat_buffer->count_lines(0, chat_buffer->length());
    chat_display->scroll(lines, 0);
    F1::unlock();
}
