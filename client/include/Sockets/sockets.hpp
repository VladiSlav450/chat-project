// client/include/Sockets/sockets.hpp

#ifndef SOCKETS_HPP
#define SOCKETS_HPP

class FdHandler;

enum { max_line_length = 1023 };

class EventSelector
{
    FdHandler *pointer_fd;
    char buffer[max_line_length];
public:
    EventSelector() : pointer_fd(0), quit_flag(false) {}
    ~EventSelector();

    void BreakLoop() { quit_flag = true; }

    void Run();
};

class FdHandler
{
    int fd;
    bool own_fd;
public:
    FdHandler(int a_fd, bool own) : fd(a_fd), own_fd(own) {}
    ~FdHandler();
    virtual void Handle(bool r, bool w) = 0;

    int GetFd() const { return fd; }
};

#endif // SOCKETS_HPP
