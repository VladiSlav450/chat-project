server

Multiprocessor server with support for multiprotocol clients

### The server supports two types of connections:
1. Chat clients - use their own chat protocol
2. HTTP clients - standard web requests

### Server architecture:
1. Main process:
- Creates three workflows via fork()
- Establishes IPC via socketpair
- Accepts connections via accept()
- Distributes clients between workflows using SCM_RIGHTS
- Implements load balancing

2. Work processes:
   - Determine the type of client:
     - Chat clients: send `CHAT_PROTOCOL` when connected
     - HTTP clients: defined by the structure of the request
- Handle clients:
- For chat clients: route messages between participants
     - For HTTP clients: 
       - Read the route configuration
       - They send static files or generate responses
       - Support configuration caching
   - Manage the lifecycle of sessions

### HTTP Routing configuration:
The server uses a configuration file in the format:

example:
[received http request] = [file path]
/about = ../lib/about.html
/contact = ../lib/contact.html
/style = ../lib/style.css

the config.conf search is performed relative to where the ./server from server/src is running.
To change the search in config.conf, you need to change the PATHCONFIGFILE variable in
the server/include/ConstantsAndVaribles.hpp file.
Pathfinding is performed depending on where the program is running.

### Commands for chat clients:
/help - what commands are there
/users - how many are online, will output only those who are in this process(( - incomplete
/name_users - names of those who are online, will output only those who are in this process(( - incomplete 
/change_name - change the name
/quit - exit the chat
/shutdown - shut down the server, done to check the correctness of data release

port = 7777

1. cd src/
2. make
3. ./server


compatible with telnet
Example:
$telnet lochalhost 7777
CHAT_PROTOCOL
/*further communication*/
