TCP chat client.

i:p = 127.0.0.1
port = 7777

1) cd client/src/
2) make
3) ./client_chat

- Implemented basic TCP client using POSIX sockets and select()  
- Added Telnet-compatible message formatting (\r\n)  
- Handles server connection/disconnection  
- Supports basic message exchange  
- TODO: Refactor into smaller methods, add error recovery  
