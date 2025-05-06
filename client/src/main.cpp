// client/src/main.cpp

#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>  
#include <cstring> 


#include  "../include/Sockets/sockets.hpp"

int main()
{
    const char *ip = "127.0.0.1";
    int port = 7777;
    size_t max_len_buf = 1023;
    int i;

    char buffer[max_len_buf];
    
    ChatClient client;
    if(!client.Connected(ip, port))
       return 1; 

    while(client.IsConnected())
    {
        printf("Bвведите сообщение: ");
        if(!fgets(buffer, max_len_buf, stdin))    
            break;

        for(i = 0; i < max_len_buf; i++)
        {
            if(buffer[i] == '\n')
                buffer[i] = '\0';
        }
            
        client.Send(buffer);
        client.Read(buffer, max_len_buf);

        printf("%s\n", buffer);
    }

    return 0;
}
