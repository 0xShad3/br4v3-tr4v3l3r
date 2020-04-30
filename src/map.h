#ifndef MAP_H_
#define MAP_H_

#include "monster.h"
#include "chest.h"

#define CSV_LN 15

#define MAP_HEIGHT 50
#define MAP_WIDTH 51

/* map game characteristics*/
#define MAP_W_SYMBOL '*'
#define MAP_P_SYMBOL ' '



/*
	Main map structure
*/
typedef struct map_t
{	
	int is_multiplayer;
	char filename[CSV_LN];
	int level;
	int chests_num;
	int monsters_num;
	int check_sum;
	char map_array[MAP_WIDTH][MAP_HEIGHT];
}map_t;


void init_map(map_t *map,monster_t mons_array[],chest_t chest_arr[]);
void print_map(map_t *map);
void map_parser(map_t* map,monster_t mons_array[],chest_t chest_arr[]);
void map_constr_fn(map_t *map);
int map_set(map_t* map,char symbol,int x, int y);



#endif // MAP_H_