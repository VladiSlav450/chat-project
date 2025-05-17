// client/include/exception.hpp

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <cstdio>

class Error
{
    char *comment;
    int err_code;
public:
    Error() : comment(0), err_code(0) {}
    Error(const char *cmt, int err_c);
    Error(const Error& other);
    virtual ~Error();

    Error& operator=(const Error& other);

    inline char *GetComment() const { return comment; }
    inline int GetErrCode() const { return err_code; }
protected:
    static char *strdup(const char *str); 
};

class ExternalError : public Error
{
    int system_error_;
public:
    ExternalError() : system_error_(0) {}
    ExternalError(const char *cmt, int err_c, int errno_code) : Error(cmt, err_c), system_error_(errno_code) {}
   
    inline int GetErrnoCode() const { return system_error_; }
};

class InputError : public Error
{
public:
    InputError() {}
    InputError(const char *cmt, int err_code) : Error(cmt, err_code) {}
};

class PortInputError : public InputError
{
    char *invalid_port_;
public:
    PortInputError() : invalid_port_(0) {}
    PortInputError(const char *cmt, int err_code, const char *port) : InputError(cmt, err_code), invalid_port_(strdup(port)) {}
    PortInputError(const PortInputError& other);
    ~PortInputError();

    PortInputError& operator=(const PortInputError& other);

    inline const char *GetInvalidPort() const { return invalid_port_; }
};

class IpInputError : public InputError
{
    char *invalid_ip_;
public:
    IpInputError() : invalid_ip_(0) {}
    IpInputError(const char *cmt, int err_code, const char *ip) : InputError(cmt, err_code), invalid_ip_(strdup(ip)) {}
    IpInputError(const IpInputError& other);
    ~IpInputError();

    IpInputError& operator=(const IpInputError& other);

    inline const char *GetInvalidIp() const { return invalid_ip_; }
};

#endif // EXCEPTION_HPP
