#ifndef SRV_INTERACT_H_
#define SRV_INTERACT_H_
#define SOCK_BUFFER_SIZE 1024
#define MAX_CLIENTS 4

#include "../src/client.h"
static int uid = 10;
client_t *clients[MAX_CLIENTS];
static _Atomic unsigned int cli_count = 0;
int search_hash(char *client_hash);
int register_hash(char *client_hash);
void broadcast_packet(char *s, int uid);
void str_trim_lf(char *arr, int length);
void print_client_addr(struct sockaddr_in addr);void queue_add(client_t *cl);
void queue_add(client_t *cl);
void queue_remove(int uid);
#endif