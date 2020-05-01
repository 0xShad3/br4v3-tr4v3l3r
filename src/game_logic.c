#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
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
    char key_press = ' ';
    char key[2];
    map_t map;
    player_t player;
    map.level = 8;
    monster_t *mons_arr = (monster_t *)calloc(sizeof(monster_t), map.level + 3);
    chest_t *chest_arr = (chest_t *)calloc(sizeof(chest_t), map.level);

    system("clear");

    init_player(&player);
    init_map(&map, mons_arr, chest_arr);
    map_set(&map, player.psymbol, player.x, player.y);
    
    
    while (1)
    {   
        
        key_press = key_input(key);
        if(key_press != ' '){
            player.prev_direction = player.direction;
            player.direction = key_press;
            move(&map,&player);
        }
        system("clear");
        print_map(&map);
        sleep(1);
    }
    
    
    free(mons_arr);
    mons_arr = NULL;
    free(chest_arr);
    chest_arr = NULL;
}

void to_print(map_t *map, player_t player, monster_t *monsters[], chest_t *chests[])
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

char key_input(char* key)
{
    int flags;
    key[0]=' ';
    flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, flags|O_NONBLOCK);

    // To get the input from the terminal without pressing the enter button
    system ("/bin/stty raw");
    read(STDIN_FILENO, key, 2);
    // Reset the "system ("/bin/stty raw")" to display correctly the output
    system ("/bin/stty cooked");

    return key[0];
}