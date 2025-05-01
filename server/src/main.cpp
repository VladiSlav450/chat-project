
#include <iostream>
#include "../include/ServerSocket.h"
#include "../include/ExceptionError/Error.h"
int main()
{
    try 
    {
        TCPServer server(8080);
        server.Start();
             
    } 
    cath (Error& error)
    {
        std::cout << "Code error: " << error.GetErrorCode() << " Message: " << error.GetMessage() << std::endl;
    }

    return 0
}
