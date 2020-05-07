#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
#include <netinet/in.h>

#include "reply.h"

#define SOCK_BUFFER_SIZE 1024
#define DEFAULT_PORT 8888

int main(int argc, char **argv)
{
    char msg[SOCK_BUFFER_SIZE];
    int sockfd;
    int socket_operation = 1;
    struct sockaddr_in addr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("[ERROR] Server socket couldnt be created. Exiting");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socket_operation, sizeof(socket_operations)))
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

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) != 0)
    {
        printf("[ERROR] Socket couldn't bind to the specified port");
        exit(EXIT_SUCCESS);
    }

    if ((listen(sockfd,3)))
    /**
     * continue here
     * /



}
