// server/src/Client.cpp

#include "../include/Client.h"
#include <unistd.h>
#include <cstring>

Client::Client(int fd, struct sockaddr_in addr): socket_fd(fd), address(addr), is_connected(bool) {}

Client::~Client()
{
    Disconnect();
}

std::string Client::ReceiveData()
{
    char buffer[1024];
    ssize_t bytes_read = read(socket_fd, buffer, sizeof(buffer) - 1);
    if(bytes_read <= 0)
    {
        is_connected = false;
        return "";
    }
    buffer[bytes_read] = '\0';
    return std::string(buffer);
}

bool Client::SendData(const std::string& data)
{
    return write(socket_fd, data.c_str(), data.size()) > 0;
}

void Client::Disconnect()
{
    if(is_connected)
    {
        shotdown(socket_fd, SHUT_RDWR);
        close(socket_fd);
        is_connected = false;
    }
}


class Client
13 {
12     int socket_fd;
11     struct sockaddr_in address;
10     bool is_connected;
 9 public:
 8     Client(int fd, struct sockaddr_in addr);
 7     ~Client();
 6
 5     std::string ReceiveData();
 4     bool SendData(const std::string& data);
 3     void Disconnect();
 2     bool IsConnected() const { return is_connected; }
 1     int GetSocket() const { return socket_fd; }
23 }[
