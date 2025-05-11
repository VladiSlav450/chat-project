TCP chat client.

1) cd src/
2) make
3) ./client_chat IP PORT

- Implemented basic TCP client using POSIX sockets and select()  
- Added Telnet-compatible message formatting (\r\n)  
- Handles server connection/disconnection  
- Supports basic message exchange  
- TODO: Refactor into smaller methods, add error recovery  
