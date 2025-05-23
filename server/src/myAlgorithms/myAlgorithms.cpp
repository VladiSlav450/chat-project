// server/src/myAlgorithms/myAlgorithms.cpp

#include <stddef.h>
#include <sys/types.h>
#include <cstring>



#include "../../include/myAlgorithms/myAlgorithms.hpp"


/*  Cheking_if_a_port_value_is_valid function - gets the string by the index
 *  Parameters:
 *  port - pointer to the string (can be NULL)
 *
 *  Returns:
 *  port number - with success
 *  NULL        - if ptr == NULL
 *  -1          - in case of error
 */

ssize_t Checking_if_a_port_value_is_valid(const char *port)
{
    int i;
    ssize_t result = 0;
    bool digits_found = false;

    if(port == NULL || *port == '\0')
        return -1;

    for(i = 0; port[i]; i++)
    {
        if(port[i] == ' ')
            continue;
        if(port[i] < '0' || port[i] > '9')  
            return -1;

        digits_found = true;
        result = result * 10 + (port[i] - 48);
        if(result > 65535)
            return -1;
    }
    return digits_found ? result : -1;
}



/*  Checking_the_validity_of_the_IP_value function - gets the string by the index
 *  Parameters:
 *  ip - pointer to the string (can be NULL)
 *
 *  Returns:
 *  true    - with success
 *  false   - if ptr == -1 of in case error
 */
// "192.168.1"
bool Checking_the_validity_of_the_IP_value(const char *ip)
{
    if((ip == NULL) || (*ip == '\0') || (strlen(ip) > 15))
       return false; 

    if(strcmp(ip, "0.0.0.0") == 0)
        return false;

    int i;
    int current_octet = 0;
    int dots = 0;
    bool digit_in_octet = false;


    for(i = 0; ip[i]; i++)
    {
        if(ip[i] == '.')
        {
            if(!digit_in_octet || dots >= 3 || ip[i + 1] == '\0')
                return false;
            dots++;
            current_octet = 0;
            digit_in_octet = false;
            continue;
        }

        if(ip[i] < '0' || ip[i] > '9')
            return false;

        if(current_octet == 0 && digit_in_octet)
            return false;

        digit_in_octet = true;
        current_octet = current_octet * 10 + (ip[i] - '0');
        if(current_octet > 255)
            return false;
    }

    return (dots == 3) && digit_in_octet;
}

#endif
