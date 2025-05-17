// client/includesockets.hpp

class FdHandler;

class EventSelector
{
    FdHandler **fd_array;
    int array_len;
    int max_fd;
    bool quit_flag;
public:
    EventSelector() : fd_array(0), quit_flag(false) {}
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
    virtual bool WantWrite() const { return true; }
};
