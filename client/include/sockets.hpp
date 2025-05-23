// client/include/sockets.hpp

#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#include "myAlgorithms/myAlgorithms.hpp"

class FdHandler;

class EventSelector
{
    SparseArray<FdHandler*> fd_array;
    struct Key_Array
    {
        int value;
        Key_Array *next;
    };
    Key_Array *first;

    int max_fd;
    bool quit_flag;
public:
    EventSelector() : fd_array(0), first(0), max_fd(-1), quit_flag(false) {}
    ~EventSelector();

    void Add(FdHandler *el);
    bool Remove(FdHandler *el);

    void BreakLoop() { quit_flag = true; }
    void Run();
};

class FdHandler
{
    int fd_;
    bool own_fd_;
public:
    FdHandler(int fd, bool own) : fd_(fd), own_fd_(own) {}
    virtual ~FdHandler();

    int GetFd() const { return fd_; }

    virtual void Handle(bool re, bool we) = 0;

    virtual bool WantRead() const { return true; }
    virtual bool WantWrite() const { return false; }
};

#endif // SOCKETS_HPP
