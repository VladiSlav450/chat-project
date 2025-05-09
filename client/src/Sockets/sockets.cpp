// client/src/Sockets/sockets.cpp

#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
#include <cstring> 
#include <cerrno>


#include "../../include/Exception/exception.hpp"
#include "../../include/Sockets/sockets.hpp"
#include "../../include/myAlgorithms/myAlgorithms.hpp"


static const char server_not_connected[] = "[Server not connected!]";
static const char server_disconnected[] = "[Server dissconnected!]";
static const char server_connected[] = "[Server connected!]";

ChatClient::~ChatClient()
{
    if(is_connected)
        close(sock);
}

bool ChatClient::Connected(const char *ip, const char *str_port)
{
    int res;
    struct sockaddr_in serv_addr;
    int port;

    try
    {
    port = ValidPort(str_port);
    ValidIp(ip);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        throw ExternalError("Socket failed!", 100, errno);


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(ip);

    res = connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if(res < 0)
        throw ExternalError("Connect failed!", 101, errno); 

    is_connected = true;
    printf("%s\n", server_connected);

    Read();
    FGetsAndWrite();
    return true;
    }
    catch(const ExternalError& ex)
    {
        fprintf(stderr, "%s\nError %d: %s (errno=%s)\n", server_not_connected, ex.GetErrCode(), ex.GetComment(), strerror(ex.GetErrnoCode())); 
        return false;
    }
    catch(const PortInputError& ex)
    {
        fprintf(stderr, "%s\nError %d: %s (port = %s)\n", server_not_connected, ex.GetErrCode(), ex.GetComment(), ex.GetInvalidPort()); 
        return false;
    }
    catch(const IpInputError& ex)
    {
        fprintf(stderr, "%s\nError %d: %s (ip = %s)\n", server_not_connected, ex.GetErrCode(), ex.GetComment(), ex.GetInvalidIp()); 
        return false;
    }
}

void ChatClient::Start()
{
    is_connected = true;

    try
    {
        do {
            int max_fd;
            fd_set rds;

            FD_ZERO(&rds);

            FD_SET(sock, &rds);
            FD_SET(STDIN_FILENO, &rds);
            max_fd = (sock > STDIN_FILENO) ? sock + 1 : STDIN_FILENO + 1;

            int res = select(max_fd + 1, &rds, 0, 0, 0);

            if(res < 0 && errno != EINTR)
                throw ExternalError("Select failed", 106, errno);

            if(FD_ISSET(sock, &rds))
                Read();

            if(FD_ISSET(STDIN_FILENO, &rds))
                FGetsAndWrite();

            } while(is_connected);
    }
    catch(const ExternalError& ex) 
    {
        fprintf(stderr, "Error %d: %s (errno=%s)\n", ex.GetErrCode(), ex.GetComment(), strerror(ex.GetErrnoCode())); 
    }
} 


void ChatClient::Read()
{
    int rc = read(sock, buffer, sizeof(buffer));
    if(rc < 0)
    {
        throw ExternalError("Read failed", 103, errno);
    }
    if(rc == 0)
        is_connected = false;

    buffer[rc] = '\0';
    printf("%s", buffer);
}

void ChatClient::FGetsAndWrite()
{
    size_t len_str = FGets();

    Write(len_str);
}

size_t ChatClient::FGets()
{
    size_t len;

    if(fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        if(feof(stdin) == false)
            throw ExternalError("Fgets failed", 104, errno);
    }


    if(strcmp(buffer, "exit\n") == 0)
    {
        is_connected = false;
    }

    len = strlen(buffer);

    if(len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\r';
        buffer[len] = '\n';
        buffer[++len] = '\0';
    }
    return len;
}

void ChatClient::Write(size_t len)
{
    ssize_t res; 
    size_t total_sent = 0;

    while(total_sent < len)
    {
        res = write(sock, buffer + total_sent, len -  total_sent);

        if(res == -1)
        {
            if(errno == EINTR)
                continue;

            else if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                fd_set wrs;

                FD_ZERO (&wrs);
                FD_SET(sock, &wrs);
                 
                select(sock + 1, NULL, &wrs, NULL, NULL);
                continue;
            }
            else
            {
                throw ExternalError("Write failed", 105, errno); 
            }
        }
        total_sent += res;
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

