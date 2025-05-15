// client/src/chatclient.cpp


#include "../incude/Exception/exception.hpp"
#include "../include/myAlgorithms/myAlgorithms.hpp"
#include "../include/chatclient.hpp"


// class ChatClient
ChatClient::ChatClient(EventSelector *sel, FLTKguiSession *disp, int fd_sock) : FdHandler(fd_sock, true), the_selector(sel), fltk(disp) 
{
    the_selector->Add(this);
    the_selector->Add(fltk);
}

ChatClient::~ChatClient()
{
    the_selector->Remove(fltk);
    the_selector->Remove(this);
}

ChatClient *ChatClient::Start(EventSelector *sel, const char *ip, const char *str_port)
{
    try
    {
        int port, ls_sock, ls_disp, res;
        Display *disp;
        
        port = ValidPort(str_port);
        ValidIp(ip);

        // init socket
        ls_sock = socket(AF_INET, SOCK_STREM, 0);
        if(ls_sock == -1)
        {
            close(ls_sock)
            throw ExternalError("Socket failed!", 100, errno);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        serv_addr.sin_addr.s_addr = inet_addr(ip);

        // connect socket
        res = connect(ls_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        if(res < 0)
        {
            close(ls_sock);
            throw ExternalError("Connect failed!", 101, errno);
        }

        // init FLTK display
        disp = XOpenDisplay(0);
        if(disp == 0)
        {
            close(ls);
            throw ExternalError("Dispaly failed!", 201, errno);
        }

        ls_disp = ConnectionNumber(disp);

        FLTKguiSession display_fltk = new FLTKguiSession(ls_disp);

        if(display_fltk == -1)
            throw ExternalError("FLTK failed!", 202, errno);

        return new ChatClient(sel, display_fltk, ls_sock);
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
