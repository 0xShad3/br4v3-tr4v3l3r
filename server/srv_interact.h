#ifndef SRV_INTERACT_H_
#define SRV_INTERACT_H_
#define SOCK_BUFFER_SIZE 1024
#define MAX_CLIENTS 4

#include "../src/client.h"
client_t *clients[MAX_CLIENTS];
int name_array[3];
int search_hash(char *client_hash);
int search_hash_id(char *client_hash);
int register_hash(char *client_hash);
void broadcast_packet(char *s, int uid);
void str_trim_lf(char *arr, int length);
void print_client_addr(struct sockaddr_in addr);
void queue_add(client_t *cl);
void queue_remove(int uid);
void queue_print();
void loginfo(int uid, char *buff);
char *on_load_map(int level);
char *servside_constr_save_filename();
int save_game_hash(char *hash);
char *search_to_load(char *filename);

#endif