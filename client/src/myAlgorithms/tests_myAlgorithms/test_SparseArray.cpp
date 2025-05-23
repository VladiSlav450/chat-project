#include <stdio.h>
#include <string.h>


#include "../../../include/myAlgorithms/myAlgorithms.hpp"

class MyStr
{
    char *p;
public:
    MyStr()
    {
        p = new char(1);
        *p = 0;
    }
    MyStr(const char *s)
    {
        int len = strlen(s);
        p = new char[len + 1];
        strcpy(p, s);
    }
    MyStr(const MyStr &s)
    {
        int len = strlen(s.p);
        p = new char[len + 1];
        strcpy(p, s.p);
    }
    ~MyStr() { delete[] p; }

    void operator=(const MyStr& s)
    {
        delete[] p;
        int len = strlen(s.p);
        p = new char[len + 1];
        strcpy(p, s.p);
    }
    const char *str() const  { return p; }
    bool operator==(const MyStr &s) const
    {
        return strcmp(p, s.p) == 0;
    }
};

void do_query(SparseArray<MyStr> &arr, char *str)
{
    int ind, r;
    r = sscanf(str, "%d", &ind);
    if(r != 1)
    {
        printf("Invalid index\n");
        return;
    }
    MyStr s = arr[ind];
    printf("** array[%d] == %s\n", ind, s.str());
}

void do_assign(SparseArray<MyStr> &arr, char *str)
{
    int ind, r;
    char val[1024];
    r = sscanf(str, "%d %1023s", &ind, val);
    if(r != 2)
    {
        printf("Invalid number\n");
        return;
    }
    arr[ind] = val;
}

int main()
{
    char buffer[1024];
    SparseArray<MyStr> array(MyStr("--//--"));
    printf("The following commands are recognized:\n"
            "   ? <key>           query the given key\n"
            "   ! <key> <value>   set the value for the key\n"
            "   #                 query the number of non-drfualt items\n"
            "   q                 quit (or just EOF)\n"
            "Please note key must be integers, suitable for 32 bit int,\n"
            "while a value may be any string not containing whitespace\n");
    while(fgets(buffer, sizeof(buffer), stdin))
    {
        switch(buffer[0])
        {
            case '?': 
                do_query(array, buffer+1);
                break;
            case '!':
                do_assign(array, buffer+1);
                break;
            case '#':
                printf("%d items now\n", array.NonzeroCount());
                break;
            case 'q':
                return 0;
            default:
                printf("Unknown action, must be '?', '!', '#' or 'q'\n");
        }
    } 
    return 0;
}
