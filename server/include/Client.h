#ifndef
#define CLIENT_H 

class Client
{
    int socket_fd;
    struct sockaddr_in address;
    bool is_connected;
public:
    Client(int fd, struct sockaddr_in addr);
    ~Client();

    void disconnected();
    bool sendData(const char* data);
    char *recevData();

    bool isConnected() const { return is_connected; }
    int getSocket() const { return socket_fd; }
};
#endif // CLIENT_H
