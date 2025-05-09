// client/src/myAlgorithms/myAlgorithms.cpp

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

ssize_t Cheking_if_a_port_value_is_valid(const char *port)
{
    int i;
    ssize_t result = 0;

    if(port == NULL)
      return NULL;
    for(i = 0; port[i]; i++)
    {
        if(!(port[i] >= '0' && port[i] <= '9' || port[i] == ' '))  
            return -1;
        if(port[i] != ' ')
           result = result * 10 + port[i];
        if(result > 65535)
            return -1;
    }
    return result;
}



/*  Checking_the_validity_of_the_IP_value function - gets the string by the index
 *  Parameters:
 *  ip - pointer to the string (can be NULL)
 *
 *  Returns:
 *  true    - with success
 *  false   - if ptr == NULL of in case error
 */

bool Checking_the_validity_of_the_IP_value(const char *ip)
{
    int i;
    if(ip == NULL)
       return false; 
    for(i = 0; ip[i]; i++)
    {
        
    }
}
 
