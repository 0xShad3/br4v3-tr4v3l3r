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
#define SINGLE_PLR 0
#define MULTI_PLR 1

void add_stats(player_t *player);
char key_input(char *key);
void to_print(map_t *map, player_t *player, monster_t monsters[], chest_t chests[]);
void to_print_multi(map_t *map,player_t players[],monster_t monsters[], chest_t chests[],int my_id);
int load_game(account_t *account, map_t *map, player_t *player, int mons_buffer[MAX_MONSTERS][MONS_ELMNTS], int chest_buffer[MAX_CHESTS]);
int load_game_multi(map_t *map, player_t players[], int monsters[MAX_MONSTERS][MONS_ELMNTS], int chests[MAX_CHESTS]);
int save_game(map_t *map, account_t *account, player_t *player, monster_t mons_arr[], chest_t chest_arr[]);
int save_game_multi(map_t *map, player_t players[], monster_t monsters[], chest_t chests[]);
void memset_arrays(int mons_buffer[][MONS_ELMNTS], int chest_buffer[MAX_CHESTS]);
void pass_object_values(monster_t mons_arr[], chest_t chest_arr[], int mons_buffer[MAX_MONSTERS][MONS_ELMNTS], int chest_buffer[MAX_CHESTS], map_t *map);
void update_objects(map_t *map, monster_t mons_arr[], chest_t chest_arr[]);
void level_up(player_t *player, monster_t monsters[], map_t *map);
void win(player_t *player);
void game_over(player_t *player);
int check_game_over_single(player_t *player);
int check_game_over_multi(player_t players[]);

void kill_all(monster_t mons_arr[], map_t *map);

#endif