#ifndef NET_PROTOCOL_H_
#define NET_PROTOCOL_H_

#include <netdb.h>
#define PACK_SIZE 256
#define SRV_BACKLOG 3
#define CLNT_BACKLOG 1

/**
 * Player
 */
typedef struct gsock_t
{
    /*set sock opt */
    int sockopt_val;
    /* bind */
    int sockfd;
    socklen_t addrlen;
    struct sockaddr_in addr;
    int bind_val;
    /* listen */
    int listen_val;
    /* Utils rw- */
    int read_val;
    int write_val;
    char buffer[PACK_SIZE];
} gsock_t;

typedef struct client_t
{
    int sockfd;
    struct sockaddr_in addr;
    int uid;
    char name[32]; /*probably not gonna need this*/
} client_t;

void set_server_socket(gsock_t *gamesocket, int port);

#endif