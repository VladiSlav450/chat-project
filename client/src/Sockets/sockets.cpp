// client/src/Sockets/sockets.cpp

#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
#include <cstring> 
#include <cerrno>


#include "../../include/Sockets/sockets.hpp"

static const char server_disconnected[] = "[Server dissconnected!]";
static const char server_connected[] = "[Server connected!]";

ChatClient::~ChatClient()
{
    if(is_connected)
        close(sock);
}

bool ChatClient::Connected(const char *ip, int port)
{
    int res;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        return false;

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip);

    res = connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(res < 0)
    {
        close(sock);
        return false;
    }

    is_connected = true;
    printf("%s\n", server_connected);

    return is_connected;
}

void ChatClient::Start()
{
    try{
        ChatClient::Read();
        ChatClient::Write();
        ChatClient::Read();
    }
    catch(const ExternalError& ex)
    {

    }

    is_connected = true;
    do {
        int max_fd;
        fd_set rds;

        FD_ZERO(&rds);

        FD_SET(sock, &rds);
        FD_SET(STDIN_FILENO, &rds);

        if(sock > STDIN_FILENO)
            max_fd = sock + 1;
        else
            max_fd = STDIN_FILENO + 1;

        int res = select(max_fd, &rds, 0, 0, 0);

        if(res < 0)
        {
            if(errno == EINTR)
                continue;
            else
                break;
        }

        if(res > 0)
        {
            if(FD_ISSET(sock, &rds))
            {
                ChatClient::Read();

            if(FD_ISSET(STDIN_FILENO, &rds))
            {
                Write();
            }
        }

    } while(is_connected);
} 

void Read()
{
    int rc = read(sock, buffer, sizeof(buffer));
    if(rc <= 0)
    {
        printf("Connection error\n");
        throw ExternalError("Read failed");
    }

    buffer[rc] = '\0';
    printf("%s\n", buffer);
}

void Write()
{
    if(!fgets(buffer, sizeof(buffer), stdin))
    {
        throw ExternalError("Fgets failed");
    }

    size_t len = strlen(buffer);

    if(strcmp(buffer, "exit\n") == 0)
    {
        is_connected = false;
    }

    buffer[len - 1] = '\r';
    buffer[len] = '\n';
    buffer[len + 1] = '\0';
    size_t sent = write(sock, buffer, len + 1);
    if(sent != len + 1) 
        throw ExternalError("Write failed");
}

