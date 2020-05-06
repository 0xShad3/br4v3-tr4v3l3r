#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "player.h"
#include "map.h"
#include "monster.h"
#include "custom_effects.h"

void init_player(player_t *player,int account_id)
{
    player->x = 18; //gia na ksekinaei apo katw aristera
    player->y = 48;
    player->health = 50;
    player->armor = 50;
    player->attack = 50;
    player->accuracy = 50;
    player->direction = UP_C;
    player->prev_direction = DOWN_C;
    player->level = 0;
    player->wins = 0;
    player->loses = 0;
    player->id = account_id;
    player->isOnline = 0;
    player->isDead = 0;
    player->name = malloc(50 * sizeof(char));
    player->pcolor = PCOLOR;
    player->psymbol = PSYMBOL;
}

void move(map_t *map, player_t *player)
{
    int temp_x = player->x;
    int temp_y = player->y;
    if (player->direction == RIGHT_C || player->direction == RIGHT_S)
    {
        if(map->map_array[player->y][player->x+1]==' '){
            player->x++;
        }
    }
    else if (player->direction == LEFT_C || player->direction == LEFT_S)
    {
        if(map->map_array[player->y][player->x-1]==' '){
            player->x--;
        }
    }
    else if (player->direction == UP_C || player->direction == UP_S)
    {
        if(map->map_array[player->y-1][player->x]==' '){
            player->y--;
        }
    }
    else if (player->direction == DOWN_C || player->direction == DOWN_S)
    {
        if(map->map_array[player->y+1][player->x]==' '){
            player->y++;
        }
    }
    map_set(map, MAP_P_SYMBOL, temp_x, temp_y);
    map_set(map, PSYMBOL, player->x, player->y);
}
/*
void attack_player(map_t *map, player_t *player,monster_t *monster)
{
    if(map->map_array[player->y+1][player->x]=='@' || map->map_array[player->y+1][player->x]=='@' || map->map_array[player->y+1][player->x]=='@' || map->map_array[player->y+1][player->x]=='@'){
        monster->health-=((player->accuracy*player->attack)/100-(monster->armor)/10);
    }
}*/


int *check_obj(map_t *map,player_t *player,int obj_array[2]){
    if(map->map_array[player->y+1][player->x]!=' ' || map->map_array[player->y+1][player->x]!='*'){
        if(map->map_array[player->y+1][player->x]=='@'){
            obj_array[0]=DOWN_OBJ;
            obj_array[1]=MONSTER;
            return obj_array;
        }else if(map->map_array[player->y+1][player->x]=='$'){
            obj_array[0]=DOWN_OBJ;
            obj_array[1]=CHEST;
            return obj_array;
        }
    }else if(map->map_array[player->y-1][player->x]!=' ' || map->map_array[player->y-1][player->x]!='*'){
        if(map->map_array[player->y-1][player->x]=='@'){
            obj_array[0]=UP_OBJ;
            obj_array[1]=MONSTER;
            return obj_array;
        }else if(map->map_array[player->y-1][player->x]=='$'){
            obj_array[0]=UP_OBJ;
            obj_array[1]=CHEST;
            return obj_array;
        }
    }else if(map->map_array[player->y][player->x-1]!=' ' || map->map_array[player->y][player->x-1]!='*'){
        if(map->map_array[player->y][player->x-1]=='@'){
            obj_array[0]=LEFT_OBJ;
            obj_array[1]=MONSTER;
            return obj_array;
        }else if(map->map_array[player->y][player->x-1]=='$'){
            obj_array[0]=LEFT_OBJ;
            obj_array[1]=CHEST;
            return obj_array;
        }
    }else if(map->map_array[player->y][player->x+1]!=' ' || map->map_array[player->y][player->x+1]!='*'){
        if(map->map_array[player->y][player->x+1]=='@'){
            obj_array[0]=RIGHT_OBJ;
            obj_array[1]=MONSTER;
            return obj_array;
        }else if(map->map_array[player->y][player->x+1]=='$'){
            obj_array[0]=RIGHT_OBJ;
            obj_array[1]=CHEST;
            return obj_array;
        }
    }
}
void die(player_t *player)
{
    player->isDead = 1;
}

void get_stats(player_t *player, monster_t monsters[],map_t *map){
    int i=0;
    printf("\033[1;33m"); //Set the text to the color red
    printf("HP: %d    ",player->health);
    printf("\033[0m");

    printf("\033[1;34m");
    printf("AC: %d    ",player->accuracy);
    printf("\033[0m");

    printf("\033[1;35m");
    printf("AR: %d    ",player->armor);
    printf("\033[0m");

    printf("\033[1;36m");
    printf("AT: %d    \n",player->attack);
    printf("\033[0m");

    printf("\033[1;32m");
    printf("Wins: ");
    printf("%d", player->wins);
    printf("\033[0m");

    printf("\033[1;31m");
    printf("   Loses: ");
    printf("%d",player->loses);
    printf("\033[0m");
    

    printf("\033[1;37m");
    printf("  Level: ");
    printf("%d\n",map->level);
    printf("\033[0m");
    

    printf("Monsters left:   [");

    for(i=0;i<map->level+3;i++){
        if(monsters[i].isDead == 0 && monsters[i].is_boss ==0){
            printf("*");
        }
        else if(monsters[i].isDead == 1){
            printf(" ");
        }
        else if(monsters[i].isDead == 0 && monsters[i].is_boss ==1 ){
            redprint_char('B');
        }
    }
    printf("]\n");
    printf("Save and exit by pressing #\n");
}

int attack(int accuracy,int damage,int armor){
    int rand_num = (rand() % (101)); 
    if(rand_num < accuracy) return (1 - (armor * 0,01) /2 ) * damage;
    else return 0; //miss
    //function for miss / hit. Might need to change later
}
/**
 * adds stats to player when opened
 * */
void open_chest(chest_t chest,player_t *player){
    chest.isOpen=1;
    player->health+=(chest.level)*10;
    player->armor+=(chest.level)*10;
    player->attack+=(chest.level)*10;
    player->accuracy+=(chest.level)*10;
}

/**
 * checks if there is an object near the player
 * when found,if it is a monster executes attack or if it is a chest opens it
 * OUSIASTIKA just attack or open chest when moving next to an object 
 * it doesn't consider the diagonal position
 **/ 
void object_found(map_t *map,player_t *player, monster_t mons_arr[], chest_t chest_arr[]){
    int i;
    int attack_val;
    for(i = 0; i < map->level + 3; i++)
    {
        if((mons_arr[i].y== player->y+1 && mons_arr[i].x==player->x) ||
            (mons_arr[i].y==player->y-1 && mons_arr[i].x==player->x) ||
            (mons_arr[i].y==player->y && mons_arr[i].x==player->x+1) ||
            (mons_arr[i].y==player->y && mons_arr[i].x==player->x-1) )
            {
                //attack_val=((player->attack)-(mons_arr[i].armor))/10;
                mons_arr[i].health -= attack(player->accuracy,player->attack,mons_arr[i].armor);
  
                if(mons_arr[i].health<=0){
                    monster_die(mons_arr[i]);
                }
            }    
    }
    for (i = 0; i < map->level; i++)
    {
        if((chest_arr[i].y== player->y+1 && chest_arr[i].x==player->x) ||
            (chest_arr[i].y==player->y-1 && chest_arr[i].x==player->x) ||
            (chest_arr[i].y==player->y && chest_arr[i].x==player->x+1) ||
            (chest_arr[i].y==player->y && chest_arr[i].x==player->x-1) )
            {
                open_chest(chest_arr[i],player);
            }
    }
}
