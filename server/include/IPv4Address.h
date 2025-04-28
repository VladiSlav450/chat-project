// server/include/IPv4Address.h
#ifndef
#define IPV4ADDRESS_H 

#include <sys/socket.h>
#include <netinet/in.h>

// Класс для работы с адресацией IPv4
class IPv4Address
{
protected:
    struct sockaddr_in address;
public:
    IPv4Address(uint16_t port, const char *ip = NULL);
    ~IPv4Address();
    
    const sockaddr* getAddr() const;
    socklen_t getAdddrLen() const;
};

#endif //IPV4ADDRESS_H
