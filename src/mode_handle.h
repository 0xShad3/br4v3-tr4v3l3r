#ifndef MODE_HANDLE_H_
#define MODE_HANDLE_H_

#include "login.h"
#include "client.h"
#include "map.h"
#include "player.h"

#define SINGLE_MODE 0
#define MULTI_MODE 1

typedef struct game_t
{
    map_t map;
    player_t players[3];
    monster_t *mons_arr;
    chest_t *chest_arr;
    client_t* client;
    int boss_arr[TOTAL_LVLS][2];
    int health_holder[3];
} game_t;

void init_game_single(account_t *account, int mode);
void init_game_multi(account_t *account, client_t *client);

void *multi_game_handler(void *args);
void *multi_recv_handler(void *args);
#endif