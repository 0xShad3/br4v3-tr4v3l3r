#include <stdio.h>
#include <stdlib.h>
#include "game_logic.h"
#include "map.h"
#include "player.h"
#include "chest.h"
#include "monster.h"

/*
    Connector library
    @purpose init and run the game
*/

void init_game(int mode)
{

    map_t map;
    player_t player;
    system("clear");

    init_player(&player);
    init_map(&map);
    // Set player in map
    map_set(&map,player.psymbol,player.x,player.y);
    //struct_array = malloc(MaxNumElements * sizeof(struct data));


}

void to_print(map_t* map,player_t player,monster_t* monsters[], chest_t* chests[])
{
    /* 
        code that prints all the compontents
        components are updated in the while true look, length for the monsters/chest arrays 
        can be found using level(map) characteristics (check struct)


        also accuracy damage healt etc...
    */
}

/*
    #SINGLE_PLAYER
    TODO:
        -game over
        -level up
             before you level up you have to free or realloc
             both arrays of structs monsters/chests to give them values again
        - battle system
    #MULTIPLAYER
    TODO:
        -everything
*/