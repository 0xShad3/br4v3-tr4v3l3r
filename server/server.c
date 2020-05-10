#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include "conn_handle.h"

#define SOCK_BUFFER_SIZE 1024
#define DEFAULT_PORT 8888

int main(int argc, char **argv)
{
    char msg[SOCK_BUFFER_SIZE];
    int srv_sockfd, cli_sockfd;
    int socket_operation = 1;

    struct sockaddr_in addr, cli;
    socklen_t cli_len;
    srv_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (srv_sockfd == -1)
    {
        printf("[ERROR] Server socket couldnt be created. Exiting");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(srv_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socket_operation, sizeof(socket_operation)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    /**
     * Clear out the area of memory where the addr is allocated
     */
    bzero(&addr, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(DEFAULT_PORT);
    /**
     * Set padding bits in heap struct area to 0
     */
    memset(addr.sin_zero, '\0', sizeof addr.sin_zero);

    if (bind(srv_sockfd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
    {
        printf("[ERROR] Socket couldn't bind to the specified port");
        exit(EXIT_SUCCESS);
    }

    if (!listen(srv_sockfd, 3))
    {
        printf("[info] Server is listening for new connections on port %d", DEFAULT_PORT);
    }
    else
    {
        printf("[ERROR] Server couldnt start listening!");
    }

    cli_len = sizeof cli;
    cli_sockfd = accept(srv_sockfd, (struct sockaddr *)&cli, &cli_len);
    
    func(cli_sockfd);
    /**
     * Threading has to go here
     */


    

}
