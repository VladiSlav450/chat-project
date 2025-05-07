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
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        return false;

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip);

    if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
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
    is_connected = true;
    do {
        int i;
        fd_set rds;
        FD_ZERO(&rds);

        FD_SET(sock, &rds);
        FD_SET(STDIN_FILENO, &rds);

        int res = select(sock + 1, &rds, 0, 0, 0);
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
                int rc = read(sock, buffer, sizeof(buffer));

                if(rc < 1)
                {
                    printf("%s\n", server_disconnected);
                    is_connected = false;
                    break;
                }

                buffer[rc] = '\0';
                printf("%s\n", buffer);
            }

            if(FD_ISSET(STDIN_FILENO, &rds))
            {
                if(!fgets(buffer, sizeof(buffer), stdin))
                {
                    is_connected = false;
                    break;
                }

                for(i = 0; i < max_line_length; i++)
                {
                    if(buffer[i] == '\n')
                    {
                        buffer[i] = '\0';
                        break;
                    }
                    
                    if(strcmp(buffer, "exit") == 0)
                    {
                        is_connected = false;
                        break;
                    }
                }

                write(sock, buffer, strlen(buffer));
            }
        }

    } while(is_connected);
} 


