// server/include/ExceptionError/UserError/IncorrectedInput.h

#ifndef INCORRECTEDINPUT_H
#define INCORRECTEDINPUT_H

#include "UserError.h"

class IncorrectedInputUser : public UserError
{
    char *user_entered_data;
public:
    IncorrectedInputUser(const char* msg, const char* input_data, int code) : UserError(msg, code); // implement coping  input_data to user_entered_data   
    const char* GetUserEnteredData() const;
};


#endif // INCORRECTEDINPUT_H
