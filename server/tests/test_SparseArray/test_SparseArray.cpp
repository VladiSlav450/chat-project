// server/tests/test_SparseArray/test_SparseArray.cpp
//
// g++ -DTEST_WITH_MYSTR=1 test_SparseArray.cpp -o prog_str 
// g++ test_SparseArray.cpp -o prog_int

/*
 * Test SparseArray
 */

#include <stdio.h>


#include "../../include/myAlgorithms/myAlgorithms.hpp"

#ifdef TEST_WITH_MYSTR

static void do_query(SparseArray<MyStr, MyStr> &array, char *str)
{
    int r;
    char buff[1024];
    r = sscanf(str, "%1023s", buff);
    if(r != 1) {
        printf("Invalid index\n");
        return;
    }
    MyStr idx(buff);
    MyStr s = array[idx];
    printf("** array[%s] == %s\n", idx.str(), s.str());
}

static void do_assign(SparseArray<MyStr, MyStr> &array, char *str)
{
    int r;
    char buff_idx[1024];
    char buff_val[1024];
    r = sscanf(str, "%1023s %1023s", buff_idx, buff_val);
    if(r != 2) {
        printf("Invalid numbers\n");
        return;
    }
    MyStr idx(buff_idx);
    MyStr val(buff_val);
    array[idx] = val;
}

int main()
{
    char buf[1024];
    SparseArray<MyStr, MyStr> array(MyStr("--//--"));
    printf("The following commands are recognized:\n"
           "   ? <key>            query the given key\n"
           "   ! <key> <value>    set the value for the key\n"
           "   #                  query the number of non-default items\n"
           "   q                  quit (or just EOF\n"
           "Please note keys must be string, suitable for 1023 sumbols,\n"
           "while a value may be any string not containing whitespace\n");
    while(fgets(buf, sizeof(buf), stdin)) {
        switch(buf[0]) {
            case '?': do_query(array, buf+1); break;
            case '!': do_assign(array, buf+1); break;
            case '#':
                printf("%d items now\n", array.NonzeroCount());
                break;
            case 'q': return 0;
            default:
                printf("Unknown action, must be '?', '!', '#' or 'q'\n");
        }
    }
    return 0;
}

#else 

static void do_query(SparseArray<int, int> &array, char *str)
{
    int idx, r;
    r = sscanf(str, "%d", &idx);
    if(r != 1) {
        printf("Invalid index\n");
        return;
    }
    printf("** array[%d] == %d\n", idx, (int) (array[idx]));
}

static void do_assign(SparseArray<int, int> &array, char *str)
{
    int idx, val, r;
    r = sscanf(str, "%d %d", &idx, &val);
    if(r != 2) {
        printf("Invalid numbers\n");
        return;
    }
    array[idx] = val;
}

int main()
{
    char buf[1024];
    SparseArray<int, int> array(0);
    printf("The following commands are recognized:\n"
           "   ? <key>            query the given key\n"
           "   ! <key> <value>    set the value for the key\n"
           "   #                  query the number of non-default items\n"
           "   q                  quit (or just EOF\n"
           "Please note both keys and values must be integers, suitable\n"
           "for 32 bit int\n");
    while(fgets(buf, sizeof(buf), stdin)) {
        switch(buf[0]) {
            case '?': do_query(array, buf+1); break;
            case '!': do_assign(array, buf+1); break;
            case '#':
                printf("%d items now\n", array.NonzeroCount());
                break;
            case 'q': return 0;
            default:
                printf("Unknown action, must be '?', '!', '#' or 'q'\n");
        }
    }
    return 0;
}

#endif  // TEST_WITH_MYSTR
