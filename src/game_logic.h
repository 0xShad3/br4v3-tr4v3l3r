#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_
#include "player.h"
#include "login.h"
#include "monster.h"
#include "chest.h"
#include "map.h"


void init_game(account_t* account,int mode);
void add_stats(player_t  *player);
char key_input(char* key);
void to_print(map_t *map, player_t *player, monster_t monsters[], chest_t chests[]);
void level_up(player_t *player,monster_t monsters[], map_t *map);
void win(player_t *player);
void game_over(player_t *player);

#endif