#ifndef PLAYER_H_
#define PLAYER_H_

#include "map.h"
#include "monster.h"
#include "client.h"
#define PCOLOR "[0;34m"
#define PSYMBOL 'Y'

#define UP_C 'W'
#define UP_S 'w'

#define DOWN_C 'S'
#define DOWN_S 's'

#define LEFT_C 'A'
#define LEFT_S 'a'

#define RIGHT_C 'D'
#define RIGHT_S 'd'

#define MONSTER 1 //gia thn check_obj
#define CHEST 2
#define UP_OBJ 1
#define DOWN_OBJ 2
#define LEFT_OBJ 3
#define RIGHT_OBJ 4

typedef struct player_t
{
    int x;
    int y;
    int health;
    int armor;
    int attack;
    int accuracy;
    char direction;
    char prev_direction;
    int level;
    int wins;
    int loses;
    int id;
    int isOnline;
    int isDead;
    char *name;
    char *pcolor;
    char psymbol;
    int prev_x;
    int prev_y;

} player_t;

void init_player(player_t *player, int account_id, int game_mode);
void move(map_t *map, player_t *player);
void move_multi(map_t *map,player_t players[],int id);
void object_found(map_t *map, player_t *player, char key_press, monster_t mons_arr[], chest_t chest_arr[]);
int *check_obj(map_t *map, player_t *player, int obj_array[2]);
void player_die(player_t *player);
void get_stats(player_t *player, monster_t monsters[], map_t *map);
void get_stats_multi(player_t players[], monster_t monster[], map_t *map, int my_id);
int attack(float accuracy, float damage, float armor);
void open_chest(chest_t chest, player_t *player);
int check_level_up(monster_t mons_arr[], map_t *map);
void player_check_max_stats(player_t *player);
void object_found_multi(client_t *client ,map_t *map, player_t *player, char key_press, monster_t mons_arr[], chest_t chest_arr[]);

#endif