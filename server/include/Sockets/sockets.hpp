// server/include/Sockets/sockets.hpp

#ifndef SOCKETS_H
#define SOCKETS_H

#include "../myAlgorithms/myAlgorithms.hpp"

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
    int fd;
    bool own_fd;
public:
    FdHandler(int a_fd, bool own) : fd(a_fd), own_fd(own) {}
    virtual ~FdHandler();

    virtual void Handle(bool re, bool wr) = 0;

    int GetFd() const { return fd; }

    virtual bool WantRead() { return true; }
    virtual bool WantWrite() { return false; }
};

#endif // SOCKETS_H
