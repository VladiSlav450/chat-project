// server/iinclude/ExceptionError/Error.h

#ifndef ERROR_H
#define ERROR_H

class Error
{
protected:
    int error_code;
    char *error_message;
public:
    // Constructor and Eestrucstor
    Error(const char *err_msg, int code);  
    Error(const Error& other);
    virtual ~Error();

    // Return the error code and error message
    int GetErrorCode() const { return error_code; } 
    const char *GetMessage() const { return error_message; } 
    // strdup: the function for copying the strings 
    static char *strdup(const char *str);
};

#endif // ERROR_H
