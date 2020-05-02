#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include "game_logic.h"
#include "map.h"
#include "player.h"
#include "chest.h"
#include "monster.h"
#include "custom_effects.h"

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
    map.level = 2;
    monster_t *mons_arr = (monster_t *)calloc(sizeof(monster_t), map.level + 3);
    chest_t *chest_arr = (chest_t *)calloc(sizeof(chest_t), map.level);

    system("clear");

    init_player(&player);
    init_map(&map, mons_arr, chest_arr);
    add_stats(&player);
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
        //print_map(&map);
        to_print(&map,&player,mons_arr,chest_arr);
        sleep(1);
    }
    
    
    free(mons_arr);
    mons_arr = NULL;
    free(chest_arr);
    chest_arr = NULL;

}

void to_print(map_t *map, player_t *player, monster_t monsters[], chest_t chests[])
{
    print_map(map);
    get_stats(player,monsters,map);
}

void add_stats(player_t  *player){
    char *saving = "Please wait while we save your changes...";
    int i=0;
    int temp_health,temp_damage,temp_armor,temp_accuracy;
    int left_points = 50;
    greenprint("[+] You have 50 points to add to your stats\n");
    printf("Health: %d\nAttack: %d\nArmor: %d\nAccuracy: %d\n", player->health, player->attack, player->armor, player->accuracy);
    redprint_slow("Choose carefully...\n");

    while(left_points > 0){
        redprint("Health points: ");
        scanf("%d",&temp_health);
        left_points -= temp_health;
        printf("Points left: %d\n", left_points);

        redprint("Attack points: ");
        scanf("%d",&temp_damage);
        left_points -= temp_damage;
        printf("Points left: %d\n", left_points);
        
        redprint("Armor points: ");
        scanf("%d",&temp_armor);
        left_points -= temp_armor;
        printf("Points left: %d\n", left_points);

        redprint("Accuracy points: ");
        scanf("%d",&temp_accuracy);
        left_points -= temp_accuracy;
        printf("Points left: %d\n", left_points);

        if(left_points < 0 || left_points > 0){
            redprint("What are you trying to do... Try again!\n");
            left_points = 50;
            printf("Left points: %d\n", left_points);
        }
    }
        //finally add the points to the players stats
        player->health += temp_health;
        player->armor += temp_armor;
        player->attack += temp_damage;
        player->accuracy += temp_accuracy;
        
        greenprint("New stats:\n");
        printf("Health: %d\nAttack: %d\nArmor: %d\nAccuracy: %d\n", player->health, player->attack, player->armor, player->accuracy);


        printf("\033[0;32m"); //set color to red
        while (saving[i] != '\0')
        {

            printf("%c", saving[i]);
            fflush(stdout);
            usleep(60000);
            i++;
        }
        printf("\033[0;0m"); // color reset
        sleep(2);
           
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