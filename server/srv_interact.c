#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>

#include "../src/net_protocol.h"
#include "../src/util.h"
#include "srv_interact.h"
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;


int search_hash(char *client_hash)
{
    char *line = NULL;
    size_t len = 0;
    char *hash_buffer;
    FILE *fd = fopen("./res/hashes.csv", "r");
    if (fd == NULL)
    {
        printf("Internal error hash file may be corrupted");
    }
    while (getline(&line, &len, fd) != -1)
    {
        hash_buffer = line;
        hash_buffer[strlen(hash_buffer) - 1] = '\0';
        printf("%s|\n",hash_buffer);
        if (!strcmp(hash_buffer, client_hash))
        {
            fclose(fd);
            free(line);
            return 0;
        }
    }

    fclose(fd);
    return -1;
}

int register_hash(char *client_hash)
{
    char *line = NULL;
    size_t len = 0;
    char *hash_buffer;
    FILE *fd = fopen("./res/hashes.csv", "r");
    if (fd == NULL)
    {
        printf("Internal error hash file may be corrupted");
        return -1;
    }
    while (getline(&line, &len, fd) != -1)
    {
        
        hash_buffer = line;
        hash_buffer[strlen(hash_buffer) - 1] = '\0';
        if (!strcmp(hash_buffer, client_hash))
        {
            fclose(fd);
            free(line);
            return -1;
        }
    }
    fclose(fd);

    fd = fopen("./res/hashes.csv", "a");
    if (fd == NULL)
    {
        printf("Internal error hash file may be corrupted");
    }
    fprintf(fd, "%s\n", client_hash); //create new line + add username/password
    fclose(fd);
    return 0;
}


void broadcast_packet(char *s, int uid)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (clients[i])
        {
            if (clients[i]->uid != uid)
            {
                if (write(clients[i]->sockfd, s, strlen(s)) < 0)
                {
                    perror("ERROR: write to descriptor failed");
                    break;
                }
            }
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void str_trim_lf(char *arr, int length)
{
    int i;
    for (i = 0; i < length; i++)
    { // trim \n
        if (arr[i] == '\n')
        {
            arr[i] = '\0';
            break;
        }
    }
}

void print_client_addr(struct sockaddr_in addr)
{
    printf("%d.%d.%d.%d",
           addr.sin_addr.s_addr & 0xff,
           (addr.sin_addr.s_addr & 0xff00) >> 8,
           (addr.sin_addr.s_addr & 0xff0000) >> 16,
           (addr.sin_addr.s_addr & 0xff000000) >> 24);
}

/* Add clients to queue */
void queue_add(client_t *cl)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (!clients[i])
        {
            clients[i] = cl;
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void queue_remove(int uid)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (clients[i])
        {
            if (clients[i]->uid == uid)
            {
                clients[i] = NULL;
                break;
            }
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}


