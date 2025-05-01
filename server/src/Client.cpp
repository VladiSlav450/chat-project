// server/src/Client.cpp

#include "../include/Client.h"
#include <unistd.h>
#include <cstring>

Client::Client(int fd, struct sockaddr_in addr): socket_fd(fd), address(addr), is_connected(true) {}

Client::~Client()
{
    Disconnect();
}

std::string Client::ReceiveData()
{
    char buffer[1024];
    ssize_t bytes_read = read(socket_fd, buffer, sizeof(buffer) - 1);
    if(bytes_read <= 0)
    { is_connected = false; return ""; } buffer[bytes_read] = '\0'; return std::string(buffer);
}

bool Client::SendData(const std::string& data)
{
    return write(socket_fd, data.c_str(), data.size()) > 0;
}

void Client::Disconnect()
{
    if(is_connected)
    {
        shutdown(socket_fd, SHUT_RDWR);
        close(socket_fd);
        is_connected = false;
    }
}
