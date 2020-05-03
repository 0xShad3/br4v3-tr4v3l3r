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

/*
    Connector library
    @purpose init and run the game
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

    int mons_buffer[MONS_ELMNTS][MAX_MONSTERS];
    int chest_buffer[MAX_CHESTS];

    init_player(&player, account->id);
    /**
     *  When a save game is found load the values from that file and start
     *  the game.
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

        map_set(&map, player.psymbol, player.x, player.y);
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
        map_set(&map, player.psymbol, player.x, player.y);
    }

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
        /**
         * Here put code that determines open open chests and dead monsters
         */
        update_objects(&map, mons_arr, chest_arr);
        to_print(&map, &player, mons_arr, chest_arr);
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
int load_game(account_t *account, map_t *map, player_t *player, int mons_buffer[MONS_ELMNTS][MAX_MONSTERS], int chest_buffer[MAX_CHESTS])
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
    printf("%s\n", load_buffer);
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
    return 1;
}

void memset_arrays(int mons_buffer[MONS_ELMNTS][MAX_MONSTERS], int chest_buffer[MAX_CHESTS])
{
    int i, j;
    for (i = 0; i < MAX_CHESTS; i++)
    {
        chest_buffer[i] = 0;
    }

    for (i = 0; i < MONS_ELMNTS; i++)
    {
        for (j = 0; j < MAX_MONSTERS; j++)
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
void pass_object_values(monster_t mons_arr[], chest_t chest_arr[], int mons_buffer[MONS_ELMNTS][MAX_MONSTERS], int chest_buffer[MAX_CHESTS], map_t *map)
{
    int i, j;

    for (j = 0; j < map->level + 3; j++)
    {

        mons_arr[j].isDead = TRUE;
    }
    for (i = 0; i < MAX_MONSTERS; i++)
    {
        if (mons_buffer[i][MONS_ID] == mons_buffer[i - 1][MONS_ID] && mons_buffer[i][MONS_HP] == mons_buffer[i - 1][MONS_HP])
            break;

        for (j = 0; j < map->level + 3; j++)
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

    for (j = 0; j < map->level; j++)
    {
        chest_arr[j].isOpen = TRUE;
    }
    for (i = 0; i < MAX_CHESTS; i++)
    {
        if (!chest_buffer[i])
            break;

        for (j = 0; j < map->level; j++)
        {
            if (chest_arr[j].chest_id == chest_buffer[i]){
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
    int i, j;
    for (j = 0; j < map->level + 3; j++)
    {
        if (mons_arr[j].isDead == TRUE)
            map_set(map, MAP_P_SYMBOL, mons_arr[j].x, mons_arr[j].y);
    }
    for (j = 0; j < map->level; j++)
    {
        if (chest_arr[j].isOpen == TRUE)
            map_set(map, MAP_P_SYMBOL, chest_arr[j].x, chest_arr[j].y);
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
