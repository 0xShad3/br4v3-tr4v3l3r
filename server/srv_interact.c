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
int search_hash_id(char *client_hash)
{
    char *line = NULL;
    size_t len = 0;
    char *hash_buffer;
    int i = 0;
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
            return i;
        }
        i++;
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
                if (send(clients[i]->sockfd, s, strlen(s), 0) < 0)
                {
                    perror("ERROR: write to descriptor failed");
                    break;
                }
            }
        }
    }

    pthread_mutex_unlock(&clients_mutex);
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
    char *buffer = (char *)calloc(sizeof(char), SOCK_BUFF_SZ);
    char content[5050];
    strcpy(filename, base);
    strcat(filename, itoa(level, str, 10));
    strcat(filename, file_extension);
    FILE *fd = fopen(filename, "r");
    if (fd == NULL)
    {
        printf("[ERROR] Could not load map file !");
    }
    bzero(content, 5050);
    fread(content, sizeof(char), sizeof(content), fd);
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

char *servside_constr_save_filename()
{

    char *buffer = (char *)calloc(sizeof(char), SOCK_BUFF_SZ);
    char temp_num_cons[5];
    int temp = 0;
    int i = 0;
    int j = 0;

    for (i = 0; i < 3; i++)
    {
        for (j = i + 1; j < 3; j++)
        {
            if (name_array[i] > name_array[j])
            {
                temp = name_array[i];
                name_array[i] = name_array[j];
                name_array[j] = temp;
            }
        }
    }
    itoa(name_array[0], temp_num_cons, 10);
    strcat(buffer, temp_num_cons);
    itoa(name_array[1], temp_num_cons, 10);
    strcat(buffer, temp_num_cons);
    itoa(name_array[2], temp_num_cons, 10);
    strcat(buffer, temp_num_cons);
    strcat(buffer, "\0");
    return buffer;
}

int save_game_hash(char *hash)
{
    char *filename = servside_constr_save_filename();
    char base[] = "./saves/server/";
    char file_extension[] = ".rpg\0";
    char filename_buffer[50];

    strcpy(filename_buffer, base);
    strcat(filename_buffer, filename);
    strcat(filename_buffer, file_extension);
    pthread_mutex_lock(&clients_mutex);
    FILE *fd = fopen(filename_buffer, "w+");
    if (fd == NULL)
    {
        printf("[ERROR] There was an error trying to save the game hash");
        return -1;
    }
    fprintf(fd, "%s\n", hash);
    fclose(fd);
    pthread_mutex_unlock(&clients_mutex);

    return 0;
}

char *search_to_load(char *filename)
{
    char filename_buffer[50];
    size_t len = 0;
    char base[] = "./saves/server/";
    char file_extension[] = ".rpg\0";
    char *hash_buffer = (char *)calloc(sizeof(char), SOCK_BUFF_SZ);
    char *send_buffer = (char *)calloc(sizeof(char), SOCK_BUFF_SZ);

    strcpy(filename_buffer, base);
    strcat(filename_buffer, filename);
    strcat(filename_buffer, file_extension);

    if (access(filename_buffer, F_OK) != -1)
    {
        FILE *fd = fopen(filename_buffer, "r");
        if (fd == NULL)
        {
            printf("[ERROR] There was an error while openning the save file\n");
            return NULL;
        }
        getline(&hash_buffer, &len, fd);

        itoa(LOAD_GAME_ID, send_buffer, 10);
        hash_buffer[strlen(hash_buffer) - 1] = '\0';
        strcat(send_buffer, NET_DELIM);
        strcat(send_buffer, hash_buffer);
        return send_buffer;
    }

    return NULL;
}