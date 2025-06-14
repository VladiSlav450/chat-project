server chat

Implemented a multiprocessing server with control through the main process

Server Architecture:
1. Main process:
- Creates 3 workflows via fork()
- Installs socketpair for IPC with each worker
- Accepts new connections via accept()
- Distributes client sockets between workers via SCM_RIGHTS

2. Work processes:
   - Accept and process transmitted client sockets
- Manage the lifecycle of client sessions
   - Provide message routing between clients
- Support cross-network communication

Technical details of the implementation:
- Using sendmsg/recvmsg to transfer descriptors
- A load balancing mechanism between workers
- IPC and client connection error handling
- Correct resource release upon completion

port = 7777

1. cd src/
2. make
3. ./server_chat


compatible with telnet
Example:
$telnet lochalhost 7777
