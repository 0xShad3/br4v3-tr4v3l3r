#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>

#include "../src/net_protocol.h"
#include "../src/util.h"
#define SOCK_BUFFER_SIZE 1024
#define DEFAULT_PORT 8888

int main(int argc, char **argv)
{
    gsock_t *gamesocket;
    struct sockaddr_in client_addr;

    if (argc != 2)
    {
        fprintf(stderr, "[ERROR] Usage : %s [port number]\n or to run on default port: %s -d", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }

    if (!strcmp(argv[1], "-d" || !strcmp(argv[1], "-D")))
    {
        set_server_socket(&gamesocket, DEFAULT_PORT);
    }
    else
    {
        set_server_socket(&gamesocket, atoi(argv[1]));
    }

    /*ready to accept connections*/
    while (1)
    {
        socklen_t client_len = sizeof(client_addr);
        /*continue threadding here*/

    }
}
