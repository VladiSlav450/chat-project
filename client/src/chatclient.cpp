// client/src/chatclient.cpp

#include <FL/Fl.H>              
#include <FL/Fl_Window.H>       
#include <FL/Fl_Text_Display.H> 
#include <FL/Fl_Text_Buffer.H>  
#include <FL/Fl_Input.H>        
#include <FL/Fl_Button.H> 

#include <X11/Xlib.h>

#include <sys/socket.h>         
#include <netinet/in.h>         
#include <arpa/inet.h>          
#include <unistd.h>            
#include <fcntl.h>      
#include <cerrno>


#include "../include/Exception/exception.hpp"
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
        size_t port;
        int fd_sock, res;
        struct sockaddr_in serv_addr;
        
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

        Display *display;

        display = XOpenDisplay(0);
        if(!display)
        {
            close(fd_sock);
            throw ExternalError("Dispaly failed!", 201, errno);
        }

        FLTKguiSession *display_fltk = FLTKguiSession::Start(display);

        if(!display_fltk)
        {
            close(fd_sock);
            throw ExternalError("FLTK failed!", 202, errno);
        }

        return new ChatClient(sel, display_fltk, fd_sock);
    }
    catch(const ExternalError& ex)
    {
        fprintf(stderr, "Error %d: %s (errno=%s)\n", ex.GetErrCode(), ex.GetComment(), strerror(ex.GetErrnoCode())); 
        return 0;
    }
    catch(const PortInputError& ex)
    {
        fprintf(stderr, "Error %d: %s (port = %s)\n", ex.GetErrCode(), ex.GetComment(), ex.GetInvalidPort()); 
        return 0;
    }
    catch(const IpInputError& ex)
    {
        fprintf(stderr, "Error %d: %s (ip = %s)\n", ex.GetErrCode(), ex.GetComment(), ex.GetInvalidIp()); 
        return 0;
    }
}

void ChatClient::Handle(bool re, bool we)
{
    if(!re)
        return;

    char *buffer = new char[max_line_length + 1];
    int rc = read(GetFd(), buffer, max_line_length);

    buffer[rc - 1] = 0;
    fltk_session->Show(buffer);

    delete[] buffer;
}

void ChatClient::Send(const char *msg)
{
    size_t len = strlen(msg);
    
    char *m = new char[len + 3];
    strcpy(m, msg);
    strcat(m, "\r\n");

    ssize_t res = write(GetFd(), m, len + 2);

    if(res <= 0)
        perror("Failed write");

    delete[] m;
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
FLTKguiSession::FLTKguiSession(int fd, Fl_Window *win, Fl_Text_Buffer *ch_buf, Fl_Text_Display *chat_disp, Fl_Input *inp, Fl_Button *send_but) 
    : FdHandler(fd, true), window(win), chat_buffer(ch_buf), display(chat_disp), input(inp), send_button(send_but) 
{
    display->buffer(chat_buffer);
    display->textfont(FL_COURIER);
    display->textsize(SIZEFONT);

    send_button->callback(SendCallback, (void*)this);
    input->when(FL_WHEN_ENTER_KEY | FL_WHEN_NOT_CHANGED);
    input->callback(SendCallback, (void*)this);

    window->end();
    window->show();
    Fl::wait(0.0);
} 

FLTKguiSession::~FLTKguiSession()
{
    Fl::lock();

    if(send_button)
    {
        delete send_button;
        send_button = 0;
    }
    if(input)
    {
        delete input;
        input = 0;
    }
    if(display)
    {
        delete display;
        display = 0;
    }
    if(chat_buffer)
    {
        delete chat_buffer;
        chat_buffer = 0;
    }
    if(window)
    {
        delete window;
        window = 0;
    }

    Fl::unlock();
}

FLTKguiSession *FLTKguiSession::Start(Display *display)
{
    int fd = ConnectionNumber(display);
    fl_open_display(display);
        
    Fl_Window *win = new Fl_Window(WINDOW_W, WINDOW_H, "Client chat");                                                                        
    Fl_Text_Buffer *ch_buf = new Fl_Text_Buffer; 
    Fl_Text_Display *disp = new Fl_Text_Display(SPACING, SPACING, DISPLAY_W, DISPLAY_H); 
    Fl_Input *inp = new Fl_Input(SPACING + LABEL_W, DISPLAY_H + SPACING * 2, INPUT_W, INPUT_H, "Text:"); 
    Fl_Button *send_but = new Fl_Button(SPACING, DISPLAY_H + INPUT_H + SPACING * 3, BUTTON_W, BUTTON_H, "Say it!");
    
    if(!win || !ch_buf || !disp || !inp || !send_but)
        return 0;

    return new FLTKguiSession(fd, win, ch_buf, disp, inp, send_but);
}

void FLTKguiSession::Handle(bool re, bool we)
{
    if(!re)
        return;

   Fl::wait(0.0);
}

void FLTKguiSession::SendCallback(Fl_Widget *w, void *data)
{
    static_cast<FLTKguiSession*>(data)->PrintAndSend();
}

void FLTKguiSession::PrintAndSend()
{
    const char *message = input->value();

    if(!message)
        return;
    if(strcmp(message, "exit") == 0)
    {
        the_master->the_selector->BreakLoop();
        return;
    }

    the_master->Send(message); 
    Show(message);
    input->value("");

}

void FLTKguiSession::Show(const char *msg)
{
    chat_buffer->append(msg);
    chat_buffer->append("\n");

    int lines = chat_buffer->count_lines(0, chat_buffer->length());
    display->scroll(lines, 0);

    Fl::flush();
}
