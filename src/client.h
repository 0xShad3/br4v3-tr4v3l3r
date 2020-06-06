#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <netinet/in.h>


#define SOCK_BUFF_SZ 2048


typedef struct client_t
{
    int sockfd;
    int connect_val;
    struct sockaddr_in addr;
    int uid;
    char name[32]; /*probably not gonna need this*/
} client_t;

void sigint_handler(int exit_flag);
int connect_server(client_t* client);
#endif // client.h
