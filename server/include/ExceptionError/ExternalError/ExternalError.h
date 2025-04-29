// server/include/ExceptionError/ExternalError/ExternalError.h
#ifndef EXTERNALERROR_H
#define EXTERNALERROR_H

class ExternalError : public Error
{
    int error_errno;
public:
    ExternalError(const char* msg, int err_code, int errno_code) : Error(msg, err_code), errno_code(errno_code) {}
    int GetErrno() const;
};

#endif // EXTERNALERROR_H
