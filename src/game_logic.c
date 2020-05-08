#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include "game_logic.h"
#include "login.h"
#include "map.h"
#include "player.h"
#include "chest.h"
#include "monster.h"
#include "custom_effects.h"

char *win_msg = "Congratulations! You are a br4v3 tr4v3l3r!";

/**
 * Main game function
 * its responsible for running the game for both single and 
 * multiplayer mode
 */

void init_game(account_t *account, int mode)
{
    int i;
    char key_press = ' ';
    char key[2];
    map_t map;
    player_t player;
    monster_t *mons_arr;
    chest_t *chest_arr;

    /**
     * To pass the values from the loaded save file to current game
     */

    int mons_buffer[MAX_MONSTERS][MONS_ELMNTS];
    int chest_buffer[MAX_CHESTS];

    init_player(&player, account->id);
    /**
     *  When a save game is found load the values from that file and start
     *  the game. Else create a file for that player and start a game from level 0
     *  
     */

    if (access(account->save_file, F_OK) != -1)
    {
        /**
         * - Set buffer arrays to 0
         * - Get the values and load them to memory
         */
        memset_arrays(mons_buffer, chest_buffer);
        if (!load_game(account, &map, &player, mons_buffer, chest_buffer))
        {
            redprint("An error occured while loading your save. Exiting!");
            exit(EXIT_FAILURE);
        }
        else
        {
            /**
             * In @load_game(); map and player got their values
             * to load the values of the alive monsters and unoppened chests the
             * function @set_objects(); set objects is called
             */
            mons_arr = (monster_t *)calloc(sizeof(monster_t), map.level + 3);
            chest_arr = (chest_t *)calloc(sizeof(chest_t), map.level);
            load_map(&map, mons_arr, chest_arr);
            pass_object_values(mons_arr, chest_arr, mons_buffer, chest_buffer, &map);
            update_objects(&map, mons_arr, chest_arr);
        }

        map_set(&map, player.psymbol, player.y, player.x);
    }
    else
    {
        /**
         *  When there's not a save game set level to 1 and start a new game
         */
        map.level = 1;
        mons_arr = (monster_t *)calloc(sizeof(monster_t), map.level + 3);
        chest_arr = (chest_t *)calloc(sizeof(chest_t), map.level);
        load_map(&map, mons_arr, chest_arr);
        add_stats(&player);
        map_set(&map, player.psymbol, player.y, player.x);
    }
    /**
     * Level change loop
     */
    while (1)
    {
        /**
         * Main game loop
         */
        while (1)
        {
            /**
            * Movement keys
            */
            key_press = key_input(key);
            if (key_press == LEFT_C ||
                key_press == LEFT_S ||
                key_press == RIGHT_C ||
                key_press == RIGHT_S ||
                key_press == UP_C ||
                key_press == UP_S ||
                key_press == DOWN_C ||
                key_press == DOWN_S)
            {
                player.prev_direction = player.direction;
                player.direction = key_press;
                object_found(&map, &player,key_press, mons_arr, chest_arr);
                move(&map, &player);
            }
            /**
            * To save the game press #
            */
            else if (key_press == '#')
            {
                if (!save_game(&map, account, &player, mons_arr, chest_arr))
                {
                    redprint("CRASH ERROR!");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    system("clear");
                    redprint_slow("Your game has been saved successfully!\nC0ntr0l 1s 4n 1llus10n!\n");
                    exit(EXIT_SUCCESS);
                }
            }
            /**
             * Check if the conditions match to level up
             */
            if (!check_level_up(mons_arr, &map))
            {
                system("clear");
                kill_all(mons_arr, &map);
                level_up(&player, mons_arr, &map);
                break;
            }

            system("clear");
            /**
            * When no direction key is pressed
            */
            update_objects(&map, mons_arr, chest_arr);
            check_game_over(&player,account,mode);
            player_check_max_stats(&player);
            to_print(&map, &player, mons_arr, chest_arr);
            usleep(100000);
            fflush(stderr);
            fflush(stdin);
            fflush(stdout);
        }
        /**
         * In case were break is called the 2 arrays are getting freed and reallocated 
         * next level.
         * Also the map gets loaded and the point values are getting passed to the 2 arrays
         * 
         */
        free(mons_arr);
        mons_arr = NULL;
        free(chest_arr);
        chest_arr = NULL;
        mons_arr = (monster_t *)calloc(sizeof(monster_t), map.level + 3);
        chest_arr = (chest_t *)calloc(sizeof(chest_t), map.level);
        load_map(&map, mons_arr, chest_arr);
        update_objects(&map, mons_arr, chest_arr);
        player.x = 18;
        player.y = 48;
        save_game(&map,account,&player,mons_arr,chest_arr);
    }
}
void to_print(map_t *map, player_t *player, monster_t monsters[], chest_t chests[])
{
    print_map(map);
    get_stats(player, monsters, map);
}
/**
 * Modifies the initial values of player stats giving him a buff of 50 points
 * to share between his stats. 
 */
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

        if (left_points != 0)
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

/**
 * Save game file parser
 * @returns 0 on error
 */
int load_game(account_t *account, map_t *map, player_t *player, int mons_buffer[MAX_MONSTERS][MONS_ELMNTS], int chest_buffer[MAX_CHESTS])
{
    char load_buffer[1024];
    char line_buff[MAX_LINES][40];
    char player_buff[MAX_TOKENS][4];
    int no_chests;
    int j;
    FILE *fd = fopen(account->save_file, "r");
    if (fd == NULL)
    {
        printf("There was an error trying to load your save");
        return 0;
        exit(EXIT_FAILURE);
    }

    fread(load_buffer, 1024, sizeof(char), fd);
    char *token;
    token = strtok(load_buffer, "\n");
    int i = 0;
    while (token != NULL)
    {

        if (token != NULL)
        {
            strcpy(line_buff[i], token);
            token = strtok(NULL, "\n");
        }
        i++;
    }
    // Load the level
    map->level = atoi(line_buff[0]);

    token = strtok(line_buff[1], ",");
    i = 0;
    while (token != NULL)
    {

        if (token != NULL)
        {
            strcpy(player_buff[i], token);
        }
        token = strtok(NULL, ",");
        i++;
    }
    // Load Player statistics needed
    player->id = atoi(player_buff[0]);
    player->x = atoi(player_buff[1]);
    player->y = atoi(player_buff[2]);
    player->health = atoi(player_buff[3]);
    player->armor = atoi(player_buff[4]);
    player->attack = atoi(player_buff[5]);
    player->accuracy = atoi(player_buff[6]);
    player->wins = atoi(player_buff[7]);
    player->loses = atoi(player_buff[8]);

    // get the number of chest ids
    no_chests = atoi(line_buff[2]);

    memset(player_buff, '0', sizeof(char) * MAX_TOKENS * 4);

    token = strtok(line_buff[3], ",");
    i = 0;
    while (token != NULL)
    {
        if (token != NULL)
        {
            strcpy(player_buff[i], token);
        }
        token = strtok(NULL, ",");
        i++;
    }

    for (i = 0; i < MAX_CHESTS; i++)
    {
        if (!atoi(player_buff[i]))
            break;
        chest_buffer[i] = atoi(player_buff[i]);
    }
    memset(player_buff, '0', sizeof(char) * MAX_TOKENS * 4);
    for (j = 0; j < MAX_MONSTERS; j++)
    {
        token = strtok(line_buff[j + 1 + 4], ",");
        i = 0;
        while (token != NULL)
        {
            if (token != NULL)
            {
                strcpy(player_buff[i], token);
            }
            token = strtok(NULL, ",");
            i++;
        }
        mons_buffer[j][MONS_ID] = atoi(player_buff[MONS_ID]);
        mons_buffer[j][MONS_HP] = atoi(player_buff[MONS_HP]);
    }
    fclose(fd);
    return 1;
}

/**
 * Saves the current game with the format [id].rpg in
 * the saves directory
 * @returns 0 on error
 * @returns 1 on normal execution
 */

int save_game(map_t *map, account_t *account, player_t *player, monster_t mons_arr[], chest_t chest_arr[])
{
    char buffer[200];
    int counter = 0;
    int offset = 0;
    int i;
    FILE *fd = fopen(account->save_file, "w+");
    if (fd == NULL)
    {
        redprint("There was an error saving your game!Exiting...");
        return 0;
    }
    fprintf(fd, "%d\n", map->level);
    fprintf(fd, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", player->id, player->x, player->y, player->health, player->armor, player->attack, player->accuracy, player->wins, player->loses);
    for (i = 0; i < map->chests_num; i++)
    {
        if (chest_arr[i].isOpen == FALSE)
        {
            offset += sprintf(buffer + offset, "%d,", chest_arr[i].chest_id);
            counter++;
        }
    }
    fprintf(fd, "%d\n", counter);
    fprintf(fd, "%s", buffer);
    fseek(fd, -1, SEEK_CUR);
    fprintf(fd, "\n");
    memset(buffer, '0', sizeof(buffer));
    offset = 0;
    counter = 0;
    for (i = 0; i < map->monsters_num; i++)
    {
        if (mons_arr[i].isDead == FALSE)
        {
            offset += sprintf(buffer + offset, "%d,%d\n", mons_arr[i].health, mons_arr[i].monster_id);
            counter++;
        }
    }
    fprintf(fd, "%d\n", counter);
    fprintf(fd, "%s", buffer);
    
    
    fclose(fd);
    return 1;
}
/**
 *  Setting the values of the two arrays to 0
 */
void memset_arrays(int mons_buffer[MAX_MONSTERS][MONS_ELMNTS], int chest_buffer[MAX_CHESTS])
{
    int i, j;
    for (i = 0; i < MAX_CHESTS; i++)
    {
        chest_buffer[i] = 0;
    }

    for (i = 0; i < MAX_MONSTERS; i++)
    {
        for (j = 0; j < MONS_ELMNTS; j++)
        {
            mons_buffer[i][j] = 0;
        }
    }
}
/**
 * Passes the values from the two file to the monster and chest arrays
 * THERE IS A BUG ON THE NUMBER OF MONSTERS AND CHESTS GETTING PRINTED
 * 
 */
void pass_object_values(monster_t mons_arr[], chest_t chest_arr[], int mons_buffer[MAX_MONSTERS][MONS_ELMNTS], int chest_buffer[MAX_CHESTS], map_t *map)
{
    int i, j;

    for (j = 0; j < map->monsters_num; j++)
    {

        mons_arr[j].isDead = TRUE;
    }
    for (i = 0; i < MAX_MONSTERS; i++)
    {
        if (mons_buffer[i][MONS_ID] == mons_buffer[i - 1][MONS_ID] && mons_buffer[i][MONS_HP] == mons_buffer[i - 1][MONS_HP])
            break;

        for (j = 0; j < map->monsters_num; j++)
        {
            if (mons_arr[j].monster_id == mons_buffer[i][MONS_ID])
            {
                mons_arr[j].health = mons_buffer[i][MONS_HP];
                mons_arr[j].isDead = FALSE;
            }
        }
    }
    /**
	 * Set all chests to open
	 * Receive the ids of the unopened chests
	 * Set these chests to unopened
	 */

    for (j = 0; j < map->chests_num; j++)
    {
        chest_arr[j].isOpen = TRUE;
    }

    for (i = 0; i < MAX_CHESTS; i++)
    {
        if (chest_buffer[i] == 0 && i != 0)
        {
            break;
        }

        for (j = 0; j < map->chests_num; j++)
        {
            // printf("%d:%d\n", chest_arr[j].chest_id, chest_buffer[i]);
            if (chest_arr[j].chest_id == chest_buffer[i])
            {
                chest_arr[j].isOpen = FALSE;
            }
        }
    }
}
/**
 * Updates objects depending if they are alive or dead for monsters
 *  and opened or not for chests
 *  if the states are true set this block to path
 */
void update_objects(map_t *map, monster_t mons_arr[], chest_t chest_arr[])
{
    int i;
    for (i = 0; i < map->monsters_num; i++)
    {
        if (mons_arr[i].isDead == TRUE)
            map_set(map, MAP_P_SYMBOL, mons_arr[i].y, mons_arr[i].x);
    }
    for (i = 0; i < map->chests_num; i++)
    {
        if (chest_arr[i].isOpen == TRUE)
            map_set(map, MAP_P_SYMBOL, chest_arr[i].y, chest_arr[i].x);
    }
}

/**
 * Used to capture key input from stdin
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

void level_up(player_t *player, monster_t monsters[], map_t *map)
{
    int i;
    //go through all monsters find boss and check if it boos is dead
    for (i = 0; i < map->monsters_num; i++)
    {
        if (monsters[i].isDead == FALSE)
        {

            return;
            /*TODO:
                Place the save function here which will save the player stats
                and reload them at the next level. Ex. player hp=20 at=70 ar=65 ac=55
                These values should be saved at the .rpg file so we can keep the player`s
                stats for the next level. Each monster should be saved + players stats
            */
        }
    }
    map->level++;
    if (map->level == 11)
    {
        win(player);
    }
    player->wins++;
    player->level++;
    player->armor += 2;
    player->accuracy +=2;
    player->health += 20;
    player->attack += 2;
    player_check_max_stats(player);
    printf("\nCongrats! Level %d is next. Get ready! \n\n", map->level);
    sleep(2);
}

void win(player_t *player)
{
    int i = 0;
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
    printf("The game was created by:\n[1] %s\n[2] %s\n[3] %s\n", "Angelos Kalaitzidis", "Theodore Zisimopoulos", "Stelios Restemis");
    printf("\033[0;0m\n");
    printf("Thanks for playing. The game will exit in 5 seconds. See you soon...maybe\n");

    sleep(5);
    exit(EXIT_SUCCESS);
}

void check_game_over(player_t *player,account_t *account,int mode)
{
    if (player->isDead == TRUE)
    {   

        printf(
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼███▀▀▀██┼███▀▀▀███┼███▀█▄█▀███┼██▀▀▀┼┼┼┼\n"
            "┼┼┼┼██┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼█┼┼┼██┼██┼┼┼┼┼┼┼\n"
            "┼┼┼┼██┼┼┼▄▄▄┼██▄▄▄▄▄██┼██┼┼┼▀┼┼┼██┼██▀▀▀┼┼┼┼\n"
            "┼┼┼┼██┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼┼┼┼┼██┼██┼┼┼┼┼┼┼\n"
            "┼┼┼┼███▄▄▄██┼██┼┼┼┼┼██┼██┼┼┼┼┼┼┼██┼██▄▄▄┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼███▀▀▀███┼▀███┼┼██▀┼██▀▀▀┼██▀▀▀▀██▄┼┼┼┼┼\n"
            "┼┼┼┼██┼┼┼┼┼██┼┼┼██┼┼██┼┼██┼┼┼┼██┼┼┼┼┼██┼┼┼┼┼\n"
            "┼┼┼┼██┼┼┼┼┼██┼┼┼██┼┼██┼┼██▀▀▀┼██▄▄▄▄▄▀▀┼┼┼┼┼\n"
            "┼┼┼┼██┼┼┼┼┼██┼┼┼██┼┼█▀┼┼██┼┼┼┼██┼┼┼┼┼██┼┼┼┼┼\n"
            "┼┼┼┼███▄▄▄███┼┼┼─▀█▀┼┼─┼██▄▄▄┼██┼┼┼┼┼██▄┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼\033[0;31m██\033[0;0m┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\033[0;31m██\033[0;0m┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼\033[0;31m████▄\033[0;0m┼┼┼\033[0;31m▄▄▄▄▄▄▄\033[0;0m┼┼┼\033[0;31m▄████\033[0;0m┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼┼\033[0;31m▀▀█▄█████████▄█▀▀\033[0;0m┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\033[0;31m█████████████\033[0;0m┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\033[0;31m██▀▀▀███▀▀▀██\033[0;0m┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\033[0;31m██\033[0;0m┼┼┼\033[0;31m███\033[0;0m┼┼┼\033[0;31m██\033[0;0m┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\033[0;31m█████▀▄▀█████\033[0;0m┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\033[0;31m███████████\033[0;0m┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼\033[0;31m▄▄▄██┼┼\033[0;31m█▀█▀█┼┼\033[0;31m██▄▄▄\033[0;0m┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼\033[0;31m▀▀██\033[0;0m┼┼┼┼┼┼┼┼┼┼┼\033[0;31m██▀▀\033[0;0m┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼\033[0;31m▀▀\033[0;0m┼┼┼┼┼┼┼┼┼┼┼\033[0;31m▀▀\033[0;0m┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n"
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n\n"
        );
        redprint_slow("\tWars aren`t meant to be won\n");
        greenprint_slow("\t\tRestarting...\n");
        sleep(2);
        init_game(account,mode);
        

        /*load the player stats with the load function
        Each time player dies stats shoud be autoloaded from
        the .rpg file. The map level should be the same as last game.
        */
    }
}

void kill_all(monster_t mons_arr[], map_t *map)
{

    int i;
    for (i = 0; i < map->monsters_num; i++)
    {
        mons_arr[i].isDead = TRUE;
    }
}
