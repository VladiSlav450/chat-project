
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <csignal>
#include <cstlib>
#include <iostream>
#include "../include/ServerSocket.h"
#include "../include/ExceptionError/Error.h"
int main()
{
    pid_t pid = fork();

    if(pid == 0)
    {
        TCPServer server(8080);
        server.Start();
        exit(EXIT_SUCCESS);
    }
    else if(pid > 0)
    {
        sleep(1);

        int client_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(client_fd < 0)
            cout << "Faild socket client\n";
        struct sockaddr_in addr = {0};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

        int connect_result = connect(client_fd, (struct sockaddr *)&addr, sizeof(addr));
        if(connct_result < 0)
        {
            std::cout << "Failed connect to server!\n";
            close(client_fd);

            kill(pid, SIGTERM);
            int status;
            wait(&status);
        }
        const char* message = "Привет от клиента!";
        write(client_fd, message, strlen(message));

        char buffer[1024];
        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
        if(buffer > 0)
        {
            buffer[bytes_read] = '\0';
            std::cout << "Сервер ответил: " << buffer << std::endl;
        }
        close(client_fd);
        kill(pid, SIGTERM);
        witpid(server_pid, NULL, 0);
    }
    else
    {
        ck_abort_msg("Fork failed");
    }
    
    std::cout << "Сервер остановлен клиент получил ответ" << std::cout;


/*
    try 
    {
        TCPServer server(8080);
        server.Start();
             
    } 
    catch (Error& error)
    {
        std::cout << "Code error: " << error.GetErrorCode() << " Message: " << error.GetMessage() << std::endl;
    }
*/
    return 0;
}
