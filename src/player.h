#ifndef PLAYER_H_
#define PLAYER_H_

#include "map.h"
#include "monster.h"
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

#define MONSTER 1  //gia thn check_obj
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

} player_t;

void init_player(player_t *player,int account_id);
void move(map_t* map,player_t *player);
//void attack_player(map_t *map, player_t *player,monster_t *monster);
void object_found(map_t *map,player_t *player, monster_t mons_arr[], chest_t chest_arr[]);
int *check_obj(map_t *map,player_t *player,int obj_array[2]);
void die(player_t *player);
void get_stats(player_t *player, monster_t monsters[],map_t *map);
int attack(int accuracy,int damage,int armor);
void open_chest(chest_t chest,player_t *player);


#endif