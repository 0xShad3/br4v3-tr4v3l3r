#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "map.h"
void init_player(player_t *player)
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
    player->id = 0;
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
