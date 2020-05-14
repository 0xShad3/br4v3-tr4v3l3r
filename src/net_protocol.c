#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#include "net_protocol.h"
#include "player.h"

void set_server_socket(gsock_t *gamesock, int port)
{

    int so_option = 1;
    gamesock->sockfd = socket(AF_INET, SOCK_STREAM, getprotobyname("TCP"));

    /* check socket init error */
    if (gamesock->sockfd == -1)
    {
        fprintf(stderr, "ERROR: %s\n", strerror(-1));
        exit(EXIT_FAILURE);
    }
    bzero(&gamesock->addr, sizeof(gamesock->addr));
    gamesock->addr.sin_family = AF_INET;
    gamesock->addr.sin_addr.s_addr = INADDR_ANY;
    gamesock->addr.sin_port = htons(port);
    gamesock->addrlen = sizeof(gamesock->addr);

    /* sock opt */
    gamesock->sockopt_val = setsockopt(gamesock->sockfd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char *)&so_option, sizeof(so_option));
     if (gamesock->bind_val == -1)
    {
        fprintf(stderr, "ERROR: %s\n", strerror(-1));
        exit(EXIT_FAILURE);
    }
    /* bind to port */
    gamesock->bind_val = bind(gamesock->sockfd, (const struct sockaddr *)&gamesock->addr, gamesock->addrlen);
    if (gamesock->bind_val == -1)
    {
        fprintf(stderr, "ERROR: %s\n", strerror(-1));
        exit(EXIT_FAILURE);
    }
    /* start listening */
    gamesock->listen_val = listen(gamesock->sockfd, SRV_BACKLOG);
    if (gamesock->listen_val == -1)
    {
        fprintf(stderr, "ERROR: %s\n", strerror(-1));
        exit(EXIT_FAILURE);
    }

    printf("[STATUS] Ready to accept connections port: %d",gamesock->addr.sin_port);
}

int on_move(int sock_fd, player_t *player)
{
}