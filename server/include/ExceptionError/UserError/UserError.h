// server/include/ExceptionError/UserError/UserError.h
#ifndef USERERROR_H
#define USERERROR_H

#include "../Error.h"

class UserError : public Error
{
public:
    UserError(const char* msg, int err_code) : Error(msg, err_code) {}
};

#endif // USERERROR_H
