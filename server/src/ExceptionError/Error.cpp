// server/src/ExceptionError/Error.cpp

#include <string.h>
#include "../../include/ExceptionError/Error.h"
#include "../../include/ExceptionError/UserError/IncorrectedInput.h"
#include "../../include/ExceptionError/ExternalError/ExternalError.h"

Error::Error(const char* err_msg, int code)
{
    error_code = code;
    error_message = strdup(err_msg);
}

Error::Error(const Error& other)
{
    error_code = other.error_code;
    error_message = strdup(other.error_message);
}
Error::~Error()
{
    delete[] error_message;
}

char* Error::strdup(const char *str)
{
    char *res = new char[strlen(str) + 1];
    strcpy(res, str);
    return res;
}

IncorrectedInputUser::IncorrectedInputUser(const char* msg, const char* input_data, int code) : UserError(msg, code)
{
    user_entered_data = strdup(input_data);
}

IncorrectedInputUser::IncorrectedInputUser(const IncorrectedInputUser& other) : UserError(other.GetMessage(), GetErrorCode())
{
    user_entered_data = strdup(other.user_entered_data);
}

IncorrectedInputUser::~IncorrectedInputUser()
{
    delete[] user_entered_data;
}

