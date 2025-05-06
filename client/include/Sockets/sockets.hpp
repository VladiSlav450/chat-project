// client/include/Sockets/sockets.hpp

#ifndef SOCKETS_HPP
#define SOCKETS_HPP


class ChatClient
{
    int sock;
    bool is_connected;
public:
    ChatClient() : sock(-1), is_connected(false) {}
    
    ~ChatClient();

    bool Connected(const char *ip, int port);

    void Send(const char *massege);
    void Read(char *str, size_t size);

    bool IsConnected() const { return is_connected; }
};

#endif // SOCKETS_HPP
