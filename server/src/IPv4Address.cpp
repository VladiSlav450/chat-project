// server/src/IPv4Adderss.cpp

#include "../include/IPv4Address.h"
#include <arpa/inet.h>
#include <string.h>

IPv4Address::IPv4Address()
{
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET; 
}

IPv4Address::IPv4Address(uint16_t port, const char *ip)
{
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if(ip == NULL)
        address.sin_addr.s_addr = INADDR_ANY;
    else 
        inet_pton(AF_INET, ip, &address.sin_addr);
}

int IPv4Address::SetIPv4Address(uint16_t port, const char *ip)
{
    struct in_addr ip_value;
    if(!inet_aton(ip, &ip_valur))
        return -1;

    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(port);
    if(ip == NULL)
        this->address.sin_addr.s_addr = INADDR_ANY;
    else 
        inet_pton(AF_INET, ip, &this->address.sin_addr);
}
