// client/src/chatclient.cpp


#include "../incude/Exception/exception.hpp"
#include "../include/myAlgorithms/myAlgorithms.hpp"
#include "../include/chatclient.hpp"


// class ChatClient
ChatClient::ChatClient(NetworkManager *a_net, GUIInterface *a_gui, MessageProcessor *a_proc) : network(a_net), gui(a_gui), proccessor(a_proc) {}

ChatClient::~ChatClient()
{
    if(network)
        delete network; 
    if(gui)
        delete gui;
    if(proccessor)
        delete proccessor;
}

ChatClient *ChatClient::Start(const char *ip, const char *str_port)
{
    try
    {
        int port = ValidPort(str_port);
        ValidIp(ip);

        NetworkManager *net = new NetworkManager();
        net->Connect(ip, port);

        GUIInterface *gui = new ConsoleGUI();

        MessageProcessor *proc = new MessageProcessior();
    {
    catch(const ExternalError& ex)
    {
        fprintf(stderr, "%s\nError %d: %s (errno=%s)\n", server_not_connected, ex.GetErrCode(), ex.GetComment(), strerror(ex.GetErrnoCode())); 
        delete net;
        return 0;
    }
    catch(const PortInputError& ex)
    {
        fprintf(stderr, "%s\nError %d: %s (port = %s)\n", server_not_connected, ex.GetErrCode(), ex.GetComment(), ex.GetInvalidPort()); 
        delete net;
        return 0;
    }
    catch(const IpInputError& ex)
    {
        fprintf(stderr, "%s\nError %d: %s (ip = %s)\n", server_not_connected, ex.GetErrCode(), ex.GetComment(), ex.GetInvalidIp()); 
        delete net;
        return 0;
    }

    return new ChatClient(net, gui, proc);
}

void ChatClient::Run()
{
    
}
