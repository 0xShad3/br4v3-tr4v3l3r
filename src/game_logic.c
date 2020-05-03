#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include "unistd.h"
#include "game_logic.h"
#include "login.h"
#include "map.h"
#include "player.h"
#include "chest.h"
#include "monster.h"
#include "custom_effects.h"

char *win_msg = "Congratulations! You are a br4v3 tr4v3l3r!";

/*
    Connector library
    @purpose init and run the game
*/

void init_game(account_t *account, int mode)
{
    char key_press = ' ';
    char key[2];
    map_t map;
    player_t player;
    map.level = 1;
    monster_t *mons_arr = (monster_t *)calloc(sizeof(monster_t), map.level + 3);
    chest_t *chest_arr = (chest_t *)calloc(sizeof(chest_t), map.level);

    system("clear");

    init_player(&player, account->id);
    init_map(&map, mons_arr, chest_arr);
    // check if there is a save for that account already
    if (access(account->save_file, F_OK) != -1)
    {
        // file exists
        //load_game();
    }
    else
    {
        // it doesnt
        add_stats(&player);
        map_set(&map, player.psymbol, player.x, player.y);
    }

    //

    while (1)
    {

        key_press = key_input(key);
        if (key_press != ' ')
        {
            player.prev_direction = player.direction;
            player.direction = key_press;
            move(&map, &player);
        }
        system("clear");
        to_print(&map, &player, mons_arr, chest_arr);
        usleep(500000);
    }

    free(mons_arr);
    mons_arr = NULL;
    free(chest_arr);
    chest_arr = NULL;
}

void to_print(map_t *map, player_t *player, monster_t monsters[], chest_t chests[])
{
    print_map(map);
    get_stats(player, monsters, map);
}

void add_stats(player_t *player)
{
    char *saving = "Please wait while we save your changes...";
    int i = 0;
    int temp_health, temp_damage, temp_armor, temp_accuracy;
    int left_points = 50;
    greenprint("[+] You have 50 points to add to your stats\n");
    printf("Health: %d\nAttack: %d\nArmor: %d\nAccuracy: %d\n", player->health, player->attack, player->armor, player->accuracy);
    redprint_slow("Choose carefully...\n");

    while (left_points > 0)
    {
        redprint("Health points: ");
        scanf("%d", &temp_health);
        left_points -= temp_health;
        printf("Points left: %d\n", left_points);

        redprint("Attack points: ");
        scanf("%d", &temp_damage);
        left_points -= temp_damage;
        printf("Points left: %d\n", left_points);

        redprint("Armor points: ");
        scanf("%d", &temp_armor);
        left_points -= temp_armor;
        printf("Points left: %d\n", left_points);

        redprint("Accuracy points: ");
        scanf("%d", &temp_accuracy);
        left_points -= temp_accuracy;
        printf("Points left: %d\n", left_points);

        if (left_points < 0 || left_points > 0)
        {
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

char key_input(char *key)
{
    int flags;
    key[0] = ' ';
    flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // To get the input from the terminal without pressing the enter button
    system("/bin/stty raw");
    read(STDIN_FILENO, key, 2);
    // Reset the "system ("/bin/stty raw")" to display correctly the output
    system("/bin/stty cooked");

    return key[0];
}

void level_up(player_t *player,monster_t monsters[], map_t *map){
    int i;
    //go through all monsters find boss and check if it boos is dead
    for(i=0;i<map->level+3;i++){
        if((monsters[i].is_boss == 1) && (monsters[i].isDead == 1)){
            map->level++;
            if(map->level == 11) win(player);
            player->wins++;
            player->level++;
            map->level++;
            printf("\nCongrats! Level %d is next. Get ready! \n\n",map->level);
            sleep(2);
            /*TODO:
                Place the save function here which will save the player stats
                and reload them at the next level. Ex. player hp=20 at=70 ar=65 ac=55
                These values should be saved at the .rpg file so we can keep the player`s
                stats for the next level. Each monster should be saved + players stats
            */
        }
    }
    return;
}

void win(player_t *player){
    int i=0;
    system("clear");
    printf("\033[0;31m"); //set color to red
    while (win_msg[i] != '\0')
    {

        printf("%c", win_msg[i]);
        fflush(stdout);
        usleep(80000);
        i++;
    }
    printf("\033[0;0m\n\n"); // color reset

    printf("\033[1;33m");
    printf("The game was created by:\n[1] %s\n[2] %s\n[3]%s\n","Angelos Kalaitzidis","Theodore Zisimopoulos","Stelios Restemis");
    printf("\033[0;0m\n");
    printf("Thanks for playing. The game will exit in 5 seconds. See you soon...maybe\n");
    
    sleep(5);
    exit(EXIT_SUCCESS);
}

void game_over(player_t *player){
    if (player->isDead == 1){
        /*load the player stats with the load function
        Each time player dies stats shoud be autoloaded from
        the .rpg file. The map level should be the same as last game.
        */
    }
}