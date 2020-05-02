#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_
#include "player.h"
#include "monster.h"
#include "chest.h"
#include "map.h"

void init_game(int mode,int account_id);
void add_stats(player_t  *player);
char key_input(char* key);
void to_print(map_t *map, player_t *player, monster_t monsters[], chest_t chests[]);
#endif