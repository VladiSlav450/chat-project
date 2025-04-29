// server/include/ExceptionError/Error.h
#ifndef ERROR_H
#define ERROR_H

class Error
{
protected:
    int error_code;
    char *error_message;
public:
    Error(const char* err_msg, int code) : error_code(code); // implement coping err_msg to error_message 
    Error(const Error& other);
    virtual ~Error();
    virtual int GetErrorCode() const;
    virtual const char *GetMessage() const; 
}

#endif // ERROR_H
