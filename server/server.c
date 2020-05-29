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

#include "srv_interact.h"

#include "../src/util.h"
#include "../src/net_protocol.h"
#include "../src/events_handler.h"
#define BUFFER_SZ 2048
#define BACKLOG 4

int connections_flag = 0;
void *handle_client(void *arg);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *ip = "127.0.0.1";
    int port = atoi(argv[1]);
    int option = 1;
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    pthread_t tid;

    /* Socket settings */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    /* Ignore pipe signals */
    signal(SIGPIPE, SIG_IGN);

    if (setsockopt(listenfd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char *)&option, sizeof(option)) < 0)
    {
        perror("ERROR: setsockopt failed");
        return EXIT_FAILURE;
    }

    /* Bind */
    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR: Socket binding failed");
        return EXIT_FAILURE;
    }

    /* Listen */
    if (listen(listenfd, BACKLOG) < 0)
    {
        perror("ERROR: Socket listening failed");
        return EXIT_FAILURE;
    }

    printf("=== GAME SERVER IS RUNNING ===\n");

    while (1)
    {
        socklen_t clilen = sizeof(cli_addr);
        connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &clilen);

        if (cli_count + 1== MAX_CLIENTS)
        {
            printf("Max clients reached. Rejected: ");
            print_client_addr(cli_addr);
            printf(" [LOG] - Queue Full -> :%d\n", cli_addr.sin_port);
            close(connfd);
            continue;
        }

        client_t *cli = (client_t *)malloc(sizeof(client_t));
        cli->addr = cli_addr;
        cli->sockfd = connfd;
        cli->uid = uid++;
        printf("[LOG] - New Connection -> ");
        print_client_addr(cli_addr);
        printf(":%d\n", cli_addr.sin_port);

        queue_add(cli);
        pthread_create(&tid, NULL, &handle_client, (void *)cli);

        sleep(1);
    }
}
void *handle_client(void *arg)
{
    char buff_out[BUFFER_SZ];
    char name[BUFFER_SZ];
    /// KEEP FLAG TO TERMINATE CLIENT
    int leave_flag = 0;
    int check_flag;
    cli_count++;
    client_t *cli = (client_t *)arg;
    //exw peiraksei aytes tis grammes
    // Name
    recv(cli->sockfd, name, BUFFER_SZ, 0);

    char *token = strtok(name, ":");

    check_flag = atoi(token);
    token = strtok(NULL, ":");
    strcpy(buff_out, token);
    loginfo(cli->uid, buff_out);
    if (check_flag == LOGIN_ID_F)
    {
        if (!search_hash(buff_out))
        {
            bzero(buff_out, BUFFER_SZ);
            itoa(1, buff_out, 10);
            send(cli->sockfd, buff_out, sizeof(int), 0);
        }
        else
        {
            bzero(buff_out, BUFFER_SZ);
            itoa(0, buff_out, 10);
            send(cli->sockfd, buff_out, sizeof(int), 0);
        }
    }

    if (check_flag == REGISTER_ID_F)
    {
        if (!register_hash(buff_out))
        {
            bzero(buff_out, BUFFER_SZ);
            itoa(1, buff_out, 10);
            send(cli->sockfd, buff_out, sizeof(int), 0);
        }
        else
        {
            bzero(buff_out, BUFFER_SZ);
            itoa(0, buff_out, 10);
            send(cli->sockfd, buff_out, sizeof(int), 0);
            close(cli->sockfd);
            queue_remove(cli->uid);
            free(cli);
            cli_count--;
            pthread_detach(pthread_self());
            return NULL;
        }
    }
    connections_flag++;
    while (1)
    {
        // wait for the clients to connect
        if (connections_flag == 3)
        {
            bzero(buff_out, BUFFER_SZ);
            itoa(1, buff_out, 10);
            send(cli->sockfd, buff_out, sizeof(int), 0);
            break;
        }
    }

    /**
     * Send to each client his id
     */
    loginfo(cli->uid,"Receiving ID");
    bzero(buff_out,BUFFER_SZ);
    itoa(cli->uid,buff_out,10);
    send(cli->sockfd,buff_out,sizeof(int),0);
    


    bzero(buff_out, BUFFER_SZ);
    recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
    loginfo(cli->uid,buff_out);
    
    
    broadcast_packet(buff_out, uid);

    close(cli->sockfd);
    queue_remove(cli->uid);
    free(cli);
    cli_count--;
    pthread_detach(pthread_self());

    return NULL;
}
