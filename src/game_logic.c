#include <stdio.h>
#include <stdlib.h>
#include "game_logic.h"
#include "chest.h"
#include "map.h"
#include "player.h"

/*
    Connector library
    @purpose init and run the game
*/

void init_game(int mode)
{

    map_t map;
    player_t player;
    system("clear");

    init_map(&map);
    print_map(&map);
}