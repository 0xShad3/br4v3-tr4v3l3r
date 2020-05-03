#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_
#include "player.h"
#include "login.h"
#include "monster.h"
#include "chest.h"
#include "map.h"
// Maximum is 13 cause thats the number of maximum monsters that can be found on the game
#define MAX_LINES 18
#define MAX_TOKENS 15

void init_game(account_t* account,int mode);
void add_stats(player_t  *player);
char key_input(char* key);
void to_print(map_t *map, player_t *player, monster_t monsters[], chest_t chests[]);
int load_game(account_t *account, map_t *map, player_t *player, int mons_buffer[MONS_ELMNTS][MAX_MONSTERS], int chest_buffer[MAX_CHESTS]);
void memset_arrays(int mons_buffer[][MAX_MONSTERS], int chest_buffer[MAX_CHESTS]);
void pass_object_values(monster_t mons_arr[], chest_t chest_arr[], int mons_buffer[MONS_ELMNTS][MAX_MONSTERS], int chest_buffer[MAX_CHESTS], map_t *map);
void update_objects(map_t *map,monster_t mons_arr[], chest_t chest_arr[]);

#endif