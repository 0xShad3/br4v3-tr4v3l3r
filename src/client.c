#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "client.h"
#include "util.h"
#include "custom_effects.h"

int exit_flag = FALSE;

void catch_exit(int sig)
{
    exit_flag = TRUE;
}

int connect_server(client_t *client)
{
    char *localhost = "127.0.0.1";
    int port = 9999;
    struct sockaddr_in server_addr;
    signal(SIGINT, catch_exit);
    client->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(localhost);
    client->connect_val = connect(client->sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (client->connect_val == -1)
    {
        redprint("[ERROR] Server seems to be down!!\n");
        return 0;
    }
    else{
        greenprint("[INFO] Server's up! :)\n");
        return 1;
    }
}