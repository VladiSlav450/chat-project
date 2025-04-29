// server/include/ExceptionError/Error.h
#ifndef ERROR_H
#define ERROR_H

class Error
{
protected:
    int error_code;
    char *error_message;
public:
    Error(const char *err_msg, int code); // implement coping err_msg to error_message 
    Error(const Error& other);
    virtual ~Error();
    int GetErrorCode() const { return error_code; } 
    const char *GetMessage() const { return error_message; } 
private:
    static char *strdup(const char *str);
};

#endif // ERROR_H
