#ifndef NET_H
#define NET_H
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "log.h"
#define MAXLINE 8192
#define LINE 1024

class NetServer
{
public:
    NetServer();
    ~NetServer();
    bool Start();
private:
    bool Init();
private:
    struct sockaddr_in m_server_addr;
    struct sockaddr_in m_client_addr;
    socklen_t m_socketLen;
    int m_sockfd;
    int m_clientfd;
};

#endif // NET_H
