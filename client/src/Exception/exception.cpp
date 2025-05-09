// client/src/Exception/exception.cpp

#include <string.h>

#include "../../include/Exception/exception.hpp"

IpInputError::IpInputError(const IpInputError& other)
{
    invalid_ip_ = strdup(other.invalid_ip_);
}

PortInputError::PortInputError(const PortInputError& other)
{
    invalid_port_ = strdup(other.invalid_port_);
} 

PortInputError::~PortInputError()
{
    delete[] invalid_port_;
}

Error::Error(const char *cmt, int err_c)
{
    comment = strdup(cmt);
    err_code = err_c;
}

Error::Error(const Error& other)
{
    comment = strdup(other.comment);
    err_code = other.err_code;
}

Error::~Error()
{
    delete[] comment;
}

char *Error::strdup(const char *str)
{
    char *res = new char[strlen(str) + 1];
    strcpy(res, str);
    return res;
}
