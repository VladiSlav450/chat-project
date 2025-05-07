// client/include/Sockets/sockets.hpp

#ifndef SOCKETS_HPP
#define SOCKETS_HPP

enum { max_line_length = 1023 };

class ChatClient
{
    int sock;
    char buffer[max_line_length + 1];
    bool is_connected;
public:
    ChatClient() : sock(-1), is_connected(false) {}
    
    ~ChatClient();

    bool Connected(const char *ip = "127.0.0.1", int port = 7777);

    void Start();

    bool IsConnected() const { return is_connected; }
};

#endif // SOCKETS_HPP
