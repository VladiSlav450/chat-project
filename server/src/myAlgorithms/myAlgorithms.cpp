// server/src/myAlgorithms/myAlgorithms.cpp

#include <string.h>


#include "../../include/myAlgorithms/myAlgorithms.hpp"


// class String

MyStr::MyStr() 
{
    p = new char[1];
    *p = 0;
}
MyStr::MyStr(const char *str) 
{
    int len = strlen(str);
    p = new char[len + 1];
    strcpy(p, str);
}

MyStr::MyStr(const MyStr& other) 
{
    if(!other.str())
    {
        p = new char[1];
        *p = 0;
    }
    else
    {
        int len = strlen(other.str());
        p = new char[len + 1];
        strcpy(p, other.str());
    }
}

MyStr& MyStr::operator=(const MyStr& other) 
{
    if(this == &other)
        return *this;
    delete[] p;
    if(!other.str())
    {
        p = new char[1];
        *p = 0;
    }
    else
    {
        int len = strlen(other.str());
        p = new char[len + 1];
        strcpy(p, other.str());
    }
    return *this;
}
