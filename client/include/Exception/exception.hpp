// client/include/exception.hpp

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

class Error
{
    char *comment;
    int err_code;
public:
    Error(const char *cmt, int err_c);
    Error(const Error& other);
    virtual ~Error();

    inline char *GetComment() const { return comment; }
    inline int GetErrCode() const { return err_code; }

private:
    static char *strdup(const char *str); 
};

class ExternalError : public Error
{
    int errno_code;
public:
    ExternalError(const char *cmt, int err_c, int errno_code) : Error(cmt, err_c), errno_code(errno_code) {}
   
    inline int GetErrnoCode() const { return errno_code; }
};

#endif // EXCEPTION_HPP
