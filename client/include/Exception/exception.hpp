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

protected:
    static char *strdup(const char *str); 
};

class ExternalError : public Error
{
    int system_error_;
public:
    ExternalError(const char *cmt, int err_c, int errno_code) : Error(cmt, err_c), system_error_(errno_code) {}
   
    inline int GetErrnoCode() const { return system_error_; }
};

class InputError::Error
{
public:
    InputError(const char *cmt, int err_code) : Error(cmt, err_code) {}
};

class PortInputError::InputError
{
    char *invalid_port_;
public:
    PortInputError(const char *cmt, int err_code, const char *port) : InputError(cmt, err_code), invalid_port_(port) {}
    PortInputError(const PortInputError& other);
    ~PortInputError();

    inline const char *GetInvalidPort() const { return invalid_port_; }
};

class IpInputError::InputError
{
    char *invalid_ip_;
public:
    IpInputError(const char *cmt, int err_code, const char *ip) : InputError(cmt, err_code), invalid_ip_(ip) {}
    IpInputError(const IpInputErro& other);
    ~IpInputError();

    inline const char *GetInvalidIp() const { return invalid_ip_; }
};

#endif // EXCEPTION_HPP
