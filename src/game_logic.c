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
#include "events_handler.h"

char *win_msg = "Congratulations! You are a br4v3 tr4v3l3r!";

void to_print(map_t *map, player_t *player, monster_t monsters[], chest_t chests[])
{
    print_map(map, monsters);
    get_stats(player, monsters, map);
}
void to_print_multi(map_t *map, player_t players[], monster_t monsters[], chest_t chests[], int my_id)
{
    print_map(map, monsters);
    get_stats_multi(players, monsters, map, my_id);
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
    //sleep(2);
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
    int j;
    FILE *fd = fopen(account->save_file, "r");
    if (fd == NULL)
    {
        printf("There was an error trying to load your save");
        return 0;
        exit(EXIT_FAILURE);
    }

    fread(load_buffer, sizeof(char), 1024, fd);
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

int on_load_game_multi(char *filename, char *net_buffer, map_t *map, player_t players[], int monsters[MAX_MONSTERS][MONS_ELMNTS], int chests[MAX_CHESTS])
{
    char load_buffer[1024];
    char line_buff[MAX_LINES][40];
    char player_buff[MAX_TOKENS][4];
    int counter;
    int j;
    char *token;
    char *token1;
    char *content;
    char *contentMD5;
    size_t filesize = 0;
    char filename_buffer[50];
    char base[] = "./saves/multi/";
    char file_extension[] = ".rpg\0";

    strcpy(filename_buffer, base);
    strcat(filename_buffer, filename);
    strcat(filename_buffer, file_extension);
    FILE *fd = fopen(filename_buffer, "r");
    if (fd == NULL)
    {
        printf("There was an error trying to load your save");
        return 0;
        exit(EXIT_FAILURE);
    }
    //DISCARD LOAD FLAG
    strtok(net_buffer, NET_DELIM);
    /**
     * Calculate the md5 hash of the save file
     * and return the file descriptor to the start of the file
     * */

    fseek(fd, 0L, SEEK_END);
    filesize = ftell(fd);
    fseek(fd, 0L, SEEK_SET);
    content = (char *)calloc(sizeof(char), filesize);
    fread(content, sizeof(char), filesize, fd);

    contentMD5 = strmd5(content, strlen(content));
    /**
     * Compaire the two hashed and take relevant action
     */
    token = strtok(NULL, NET_DELIM);
    if (strcmp(token, contentMD5) != 0)
    {
        /**
         * On fail comparison
         */
        return 0;
    }

    fread(load_buffer, sizeof(char), 1024, fd);
    char *token2 = strtok(content, "\n");
    int i = 0;
    while (token2 != NULL)
    {

        if (token2 != NULL)
        {
            strcpy(line_buff[i], token2);
            token2 = strtok(NULL, "\n");
        }
        i++;
    }

    // Load the level
    map->level = atoi(line_buff[0]);
    for (i = 0; i < 3; i++)
    {
        token1 = strtok(line_buff[i + 1], ",");
        j = 0;
        while (token1 != NULL)
        {

            if (token1 != NULL)
            {
                strcpy(player_buff[j], token1);
            }

            token1 = strtok(NULL, ",");
            j++;
        }

        // Load Player statistics needed
        players[i].id = atoi(player_buff[0]);
        players[i].x = atoi(player_buff[1]);
        players[i].y = atoi(player_buff[2]);
        players[i].health = atoi(player_buff[3]);
        players[i].armor = atoi(player_buff[4]);
        players[i].attack = atoi(player_buff[5]);
        players[i].accuracy = atoi(player_buff[6]);
        players[i].wins = atoi(player_buff[7]);
        players[i].loses = atoi(player_buff[8]);
        players[i].isDead = atoi(player_buff[9]);
        memset(player_buff, '0', sizeof(char) * MAX_TOKENS * 4);
    }
    counter = atoi(strtok(line_buff[4], ","));
    token2 = strtok(line_buff[5], ",");
    i = 0;
    while (token2 != NULL)
    {
        if (token2 != NULL)
        {
            strcpy(player_buff[i], token2);
        }
        token2 = strtok(NULL, ",");
        i++;
    }
    for (i = 0; i < counter; i++)
    {
        chests[i] = atoi(player_buff[i]);
    }
    counter = atoi(strtok(line_buff[6], NET_DELIM));
    memset(player_buff, '0', sizeof(char) * MAX_TOKENS * 4);
    for (j = 0; j < MAX_MONSTERS; j++)
    {
        /// + 3 -> + MAX_PLAYERS
        token2 = strtok(line_buff[j + 4 + 2], ",");
        i = 0;
        while (token2 != NULL)
        {
            if (token2 != NULL)
            {
                strcpy(player_buff[i], token2);
            }
            token2 = strtok(NULL, ",");
            i++;
        }
        monsters[j][1] = atoi(player_buff[MONS_ID]);
        monsters[j][0] = atoi(player_buff[MONS_HP]);
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
int save_game_multi(char *filename, map_t *map, player_t players[], monster_t monsters[], chest_t chests[])
{
    char buffer[200];
    char filename_buffer[200];
    int counter = 0;
    int offset = 0;
    int i;
    char base[] = "./saves/multi/";
    char file_extension[] = ".rpg\0";

    strcpy(filename_buffer, base);
    strcat(filename_buffer, filename);
    strcat(filename_buffer, file_extension);

    FILE *fd = fopen(filename_buffer, "w+");
    if (fd == NULL)
    {
        redprint("There was an error saving your game!Exiting...");
        return 0;
    }
    fprintf(fd, "%d\n", map->level);

    for (i = 0; i < 3; i++)
    {
        fprintf(fd, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", players[i].id, players[i].x, players[i].y, players[i].health, players[i].armor, players[i].attack, players[i].accuracy, players[i].wins, players[i].loses, players[i].isDead);
    }
    for (i = 0; i < map->chests_num; i++)
    {
        if (chests[i].isOpen == FALSE)
        {
            offset += sprintf(buffer + offset, "%d,", chests[i].chest_id);
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
        if (monsters[i].isDead == FALSE)
        {
            offset += sprintf(buffer + offset, "%d,%d\n", monsters[i].health, monsters[i].monster_id);
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

void pass_object_values_multi(monster_t mons_arr[], chest_t chest_arr[], int mons_buffer[MAX_MONSTERS][MONS_ELMNTS], int chest_buffer[MAX_CHESTS], map_t *map)
{
    int i, j;

    for (j = 0; j < map->monsters_num; j++)
    {

        mons_arr[j].isDead = TRUE;
    }
    for (i = 0; i < map->monsters_num; i++)
    {
        if (mons_buffer[i][MONS_HP] != 0)
        {
            mons_arr[mons_buffer[i][MONS_ID]].isDead = FALSE;
            mons_arr[mons_buffer[i][MONS_ID]].health = mons_buffer[i][MONS_HP];
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
        if (chest_buffer[i] != 0)
        {
            chest_arr[i].isOpen = FALSE;
        }
        if (chest_buffer[i] == 0 && i == 0)
        {
            chest_arr[i].isOpen = FALSE;
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
        if (mons_arr[i].isDead == TRUE && map->map_array[mons_arr[i].y][mons_arr[i].x] == MSYMBOL)
            map_set(map, MAP_P_SYMBOL, mons_arr[i].y, mons_arr[i].x);
    }
    for (i = 0; i < map->chests_num; i++)
    {
        if (chest_arr[i].isOpen == TRUE && map->map_array[chest_arr[i].y][chest_arr[i].x] == CSYMBOL)
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
    fcntl(STDIN_FILENO, F_SETFL, flags);

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
    player->accuracy += 2;
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

int check_game_over_single(player_t *player)
{
    if (player->isDead == TRUE)
    {
        system("clear");

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
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n\n");
        redprint_slow("\tWars aren`t meant to be won\n");
        greenprint_slow("\t\tRestarting...\n");
        sleep(2);
        player->loses++;
        return 0;

        /*load the player stats with the load function
        Each time player dies stats shoud be autoloaded from
        the .rpg file. The map level should be the same as last game.
        */
    }
    return 1;
}

int check_game_over_multi(player_t players[], game_t *game)
{
    int i;
    if (players[0].isDead == TRUE &&
        players[1].isDead == TRUE &&
        players[2].isDead == TRUE)
    {
        system("clear");

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
            "┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n\n");
        redprint_slow("\tWars aren`t meant to be won\n");
        greenprint_slow("\t\tRestarting...\n");
        sleep(2);
        for (int i = 0; i < 3; i++)
        {
            players[i].loses++;
        }
        for (i = 0; i < game->map.level + 3; i++)
        {
            game->mons_arr[i].isDead = FALSE;
            game->mons_arr[i].health = 100;
            if (game->mons_arr[i].monster_id == game->boss_arr[game->map.level - 1][1])
            {
                game->mons_arr[i].is_boss = TRUE;
                game->mons_arr[i].health = 20 + 4 * game->map.level;
                game->mons_arr[i].armor = 20 + 4 * game->map.level;
                game->mons_arr[i].attack = 10 + 4 * game->map.level;
                game->mons_arr[i].accuracy = 10 + 4 * game->map.level;
            }
            map_set(&game->map, MSYMBOL, game->mons_arr[i].y, game->mons_arr[i].x);
        }
        for (i = 0; i < game->map.level; i++)
        {
            game->chest_arr[i].isOpen = FALSE;
            map_set(&game->map, CSYMBOL, game->chest_arr[i].y, game->chest_arr[i].x);
        }

        return 0;

        /*load the player stats with the load function
        Each time player dies stats shoud be autoloaded from
        the .rpg file. The map level should be the same as last game.
        */
    }
    return 1;
}
void kill_all(monster_t mons_arr[], map_t *map)
{

    int i;
    for (i = 0; i < map->monsters_num; i++)
    {
        mons_arr[i].isDead = TRUE;
    }
}

void on_death_hp_set(map_t *map, player_t players[])
{
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        if (players[i].isDead == TRUE)
        {
            players[i].health = 0;
        }
    }
}
