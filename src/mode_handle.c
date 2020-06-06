#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "game_logic.h"
#include "login.h"
#include "map.h"
#include "player.h"
#include "chest.h"
#include "monster.h"
#include "custom_effects.h"

#include "client.h"
#include "events_handler.h"

volatile sig_atomic_t exit_flag = FALSE;

/**
 * Main game function
 * its responsible for running the game for single player mode
 */
void init_game_single(account_t *account, int mode)
{
    char key_press = ' ';
    char key[2];
    map_t map;
    player_t player;
    monster_t *mons_arr;
    chest_t *chest_arr;
    int health_holder = 0;
    int boss_arr[TOTAL_LVLS][2];

    /**
     * To pass the values from the loaded save file to current game
     */

    int mons_buffer[MAX_MONSTERS][MONS_ELMNTS];
    int chest_buffer[MAX_CHESTS];

    /**
     * Parse boss monsters
     */
    monster_boss_parser(boss_arr);
    init_player(&player, account->id, SINGLE_MODE);
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
            load_map(&map, mons_arr, chest_arr, boss_arr);
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
        load_map(&map, mons_arr, chest_arr, boss_arr);
        add_stats(&player);
        map_set(&map, player.psymbol, player.y, player.x);
    }
    /**
     * Fetch health to the holder var
     */
    health_holder = player.health;
    /**
     * Level change loop
     */
    if (map.level == 1)
        save_game(&map, account, &player, mons_arr, chest_arr);

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
                object_found(&map, &player, key_press, mons_arr, chest_arr);
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

            if (!check_game_over(&player, mode))
            {
                ///system("clear");
                player.health = health_holder;
                player.isDead = FALSE;
                break;
            }

            /**
            * When no direction key is pressed
            */

            system("clear");
            player_check_max_stats(&player);
            to_print(&map, &player, mons_arr, chest_arr);
            update_objects(&map, mons_arr, chest_arr);
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
        load_map(&map, mons_arr, chest_arr, boss_arr);
        update_objects(&map, mons_arr, chest_arr);
        player.x = 18;
        player.y = 48;
        save_game(&map, account, &player, mons_arr, chest_arr); //this is the autosave feature for every level
    }
}

void init_game_multi(account_t *account, client_t *client)
{
    char *net_buffer;
    char net_pass_buffer[sizeof(int)];
    game_t game;
    /**
     * Parse boss monsters
     */
    monster_boss_parser(game.boss_arr);

    /**
     * 
     * receive global game id and initialize the player
     * 
     */
    recv(client->sockfd, net_pass_buffer, sizeof(int), 0);
    init_player(&game.players[0], atoi(net_pass_buffer), MULTI_MODE);
    add_stats(&game.players[0]);
    net_buffer = on_player_update_stats(&game.players[0]);
    send(client->sockfd, net_buffer, strlen(net_buffer), 0);

    /***
     * Check the hash of the file to be read
     */
    recv(client->sockfd, net_buffer, SOCK_BUFF_SZ, 0);

    if (!decode_on_map_receive(&game.map, net_buffer))
    {
        redprint("[ERROR] The map file may be corrupted or modified!");
        exit(EXIT_FAILURE);
    }

    game.mons_arr = (monster_t *)calloc(sizeof(monster_t), game.map.level + 3);
    game.chest_arr = (chest_t *)calloc(sizeof(chest_t), game.map.level);
    load_map(&game.map, game.mons_arr, game.chest_arr, game.boss_arr);

    /**
     * Fetch health to health holder
     */
    game.health_holder = game.players[0].health;
    /**
     * 
     * Need to fill players array
     */

    pthread_t game_thread;
    pthread_t recv_thread;

    if (pthread_create(&game_thread, NULL, multi_game_handler, (void *)&game) != 0)
    {
        perror("Create game thread");
        // HARD INTERRUPT SIGNAL
    }

    if (pthread_create(&recv_thread, NULL, multi_recv_handler, (void *)&game) != 0)
    {
        perror("Create receive thread");
    }

    /**
     * 
     * may need to use here pthread_cancel - pthread_cleanup_pop/push
     */

    while (1)
    {
        if (exit_flag != 0)
        {
            redprint_slow("K CYA!");
        }
    }

    close(client->sockfd);
}

void *multi_game_handler(void *args)
{

    char key_press = ' ';
    char key[2];
    game_t *game = (game_t *)args;

    while (1)
    {
        while (1)
        {
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
                game->players[0].prev_direction = game->players[0].direction;
                game->players[0].direction = key_press;
                object_found(&game->map, &game->players[0], key_press, game->mons_arr, game->chest_arr);
                move(&game->map, &game->players[0]);
            }

            /**
         * Save the game press #
         */

            // save game function

            /**
         * Check if conditions match to level up
         */
            if (!check_level_up(game->mons_arr, &game->map))
            {
                system("clear");
                kill_all(game->mons_arr, &game->map);
                level_up(&game->players[0], game->mons_arr, &game->map);
                break;
            }
            /**
         * Check if conditions match to game over
         */

            if (!check_game_over(&game->players[0], MULTI_MODE))
            {
                game->players[0].health = game->health_holder;
                game->players[0].isDead = FALSE;
                break;
            }

            /**
        * When no direction key is pressed
        */
            system("clear");
            player_check_max_stats(&game->players[0]);
            to_print(&game->map, &game->players[0], game->mons_arr, game->chest_arr);
            update_objects(&game->map, game->mons_arr, game->chest_arr);
            usleep(10000);
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
        free(game->mons_arr);
        game->mons_arr = NULL;
        free(game->chest_arr);
        game->chest_arr = NULL;
        game->mons_arr = (monster_t *)calloc(sizeof(monster_t), game->map.level + 3);
        game->chest_arr = (chest_t *)calloc(sizeof(chest_t), game->map.level);
        load_map(&game->map, game->mons_arr, game->chest_arr, game->boss_arr);
        update_objects(&game->map, game->mons_arr, game->chest_arr);
        game->players[0].x = 18;
        game->players[0].y = 48;
    }
    //to_print(&game->map, &game->players[0], game->mons_arr, game->chest_arr);

    return NULL;
}

void *multi_recv_handler(void *args)
{
    return NULL;
}