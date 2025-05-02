// server/include/IPv4Address.h
#ifndef IPV4ADDRESS_H
#define IPV4ADDRESS_H 

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdint>
#include <cstddef>

// Класс для работы с адресацией IPv4
class IPv4Address
{
protected:
    struct sockaddr_in address;
public:
    IPv4Address();
    IPv4Address(uint16_t port, const char *ip = NULL);
    
    const struct sockaddr* GetAddr() const { return (const struct sockaddr *)&address; }
    socklen_t GetAddrLen() const { return sizeof(address); }
    int SetIPv4Address(uint16_t port, const char *ip);
};

#endif //IPV4ADDRESS_H
