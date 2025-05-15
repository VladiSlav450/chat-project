// client/include/myAlgoritmics/myAlgoritmics.hpp

#ifndef MYALGORITMICS_HPP
#define MYALGORITMICS_HPP

/*  Cheking_if_a_port_value_is_valid function - gets the string by the index
 *  Parameters:
 *  port - pointer to the string (can be NULL)
 *
 *  Returns:
 *  port number - with success
 *  -1          - in case of error or port == NULL
 */

ssize_t Checking_if_a_port_value_is_valid(const char *port);


/*  Checking_the_validity_of_the_IP_value function - gets the string by the index
 *  Parameters:
 *  ip - pointer to the string (can be NULL)
 *
 *  Returns:
 *  true    - with success
 *  false   - if ptr == NULL of in case error
 */

bool Checking_the_validity_of_the_IP_value(const char *ip);


#endif // MYALGORITMICS_HPP
