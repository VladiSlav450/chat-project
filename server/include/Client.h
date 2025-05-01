// server/include/Client.h

#ifndef CLIENT_H
#define CLIENT_H 

#include <string>
#include <sys/socket.h>

class Client
{
    int socket_fd;
    struct sockaddr_in address;
    bool is_connected;
public:
    Client(int fd, struct sockaddr_in addr);
    ~Client();

    std::string ReceiveData();
    bool SendData(const std::string& data);
    void Disconnect();
    bool IsConnected() const { return is_connected; }
    int GetSocket() const { return socket_fd; }
};

#endif // CLIENT_H
