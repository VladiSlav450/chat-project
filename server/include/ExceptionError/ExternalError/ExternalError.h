// server/include/ExceptionError/ExternalError/ExternalError.h
#ifndef EXTERNALERROR_H
#define EXTERNALERROR_H

#include "../Error.h"

class ExternalError : public Error
{
    int error_errno;
public:
    ExternalError(const char* msg, int err_code) : Error(msg, err_code), error_errno(errno) {}
    int GetErrno() const { return error_errno; }
};

#endif // EXTERNALERROR_H
