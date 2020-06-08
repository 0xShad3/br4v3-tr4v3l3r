#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>

#include "../src/net_protocol.h"
#include "../src/util.h"
#include "../src/events_handler.h"
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
                if (send(clients[i]->sockfd, s, strlen(s),0) < 0)
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

void queue_print()
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i])
        {
            printf("CLIENT: \n");
            printf("uid -> %d\n", clients[i]->uid);
            printf("sockfd -> %d\n", clients[i]->sockfd);
            printf("connect_val -> %d\n", clients[i]->connect_val);
            printf("addr -> ");
            print_client_addr(clients[i]->addr);
            printf("\n\n");
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}
void loginfo(int uid, char *buff)
{
    printf("[LOG] Uid:%d -> %s\n", uid, buff);
}
char *on_load_map(int level)
{
    const char base[] = "./maps/map";
    const char file_extension[] = ".csv\0";
    char str[10];
    char level_buffer[2];
    char filename[40];
    char *buffer = (char*) calloc(sizeof(char),SOCK_BUFF_SZ);
    char content[5050];
    strcpy(filename, base);
    strcat(filename, itoa(level, str, 10));
    strcat(filename, file_extension);
    FILE *fd = fopen(filename, "r");
    if (fd == NULL)
    {
        printf("[ERROR] Could not load map file !");
    }
    bzero(content,5050);
    fread(content, sizeof(content), sizeof(char), fd);
    char *content_md5 = strmd5(content, strlen(content));
    itoa(MAP_OPEN_ID, buffer, 10);
    strcat(buffer, NET_DELIM);
    itoa(level, level_buffer, 10);
    strcat(buffer, level_buffer);
    strcat(buffer, NET_DELIM);
    strcat(buffer, content_md5);

    free(content_md5);
    content_md5 = NULL;
    fclose(fd);

    return buffer;
}