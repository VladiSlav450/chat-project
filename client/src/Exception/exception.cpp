// client/src/Exception/exception.cpp

#include <string.h>

#include "../../include/Exception/exception.hpp"

// IpInputError
IpInputError::IpInputError(const IpInputError& other) 
    : InputError(other), invalid_ip_(other.invalid_ip_ ? strdup(other.invalid_ip_) : NULL) {}

IpInputError::~IpInputError()
{
    delete[] invalid_ip_;
}

IpInputError& IpInputError::operator=(const IpInputError& other)
{
    if(this != &other)
    {
        InputError::operator=(other);
        delete[] invalid_ip_;
        invalid_ip_ = strdup(other.invalid_ip_);
    }
    return *this;
}

// PortInputError
PortInputError::PortInputError(const PortInputError& other) 
    : InputError(other), invalid_port_(other.invalid_port_ ? strdup(other.invalid_port_) : NULL) {}

PortInputError::~PortInputError()
{
    delete[] invalid_port_;
}

PortInputError& PortInputError::operator=(const PortInputError& other)
{
    if(this != &other)
    {
        InputError::operator=(other);
        delete[] invalid_port_;
        invalid_port_ = strdup(other.invalid_port_);
    }
    return *this;
}

// Error
Error::Error(const char *cmt, int err_c) : comment(NULL), err_code(err_c)
{
    if(cmt)
        comment = strdup(cmt);
}

Error::Error(const Error& other) : comment(NULL), err_code(other.err_code)
{
    if(other.comment)
        comment = strdup(other.comment);
}

Error::~Error()
{
    delete[] comment;
}

Error& Error::operator=(const Error& other)
{
    if(this != &other)
    {
        delete[] comment;
        comment = other.comment ? strdup(other.comment) : NULL;
        err_code = other.err_code;
    }
    return *this;
}

char *Error::strdup(const char *str)
{
    if(!str)
        return NULL;
    char *res = new char[strlen(str) + 1];
    strcpy(res, str);
    return res;
}
