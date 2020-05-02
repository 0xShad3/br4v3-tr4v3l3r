#include <stdio.h>
#include <stdlib.h>
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
        player->x++;
    }
    else if (player->direction == LEFT_C || player->direction == LEFT_S)
    {
        player->x--;
    }
    else if (player->direction == UP_C || player->direction == UP_S)
    {
        player->y--;
    }
    else if (player->direction == DOWN_C || player->direction == DOWN_S)
    {
        player->y++;
    }
    map_set(map, MAP_P_SYMBOL, temp_x, temp_y);
    map_set(map, PSYMBOL, player->x, player->y);
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
    printf("\033[0m");
    printf("[ %d ]", player->wins);

    printf("\033[1;31m");
    printf(" Loses: ");
    printf("\033[0m");
    printf("[ %d ]\n",player->loses);

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


}