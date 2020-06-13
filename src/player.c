#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "player.h"
#include "map.h"
#include "monster.h"
#include "client.h"
#include "custom_effects.h"
#include "events_handler.h"

void init_player(player_t *player, int account_id, int game_mode)
{
    if (!game_mode)
    {
        player->x = 18; //gia na ksekinaei apo katw aristera
    }
    else
    {
        player->x = 18 + account_id;
    }

    player->y = 48;
    player->health = 50;
    player->armor = 20;
    player->attack = 20;
    player->accuracy = 20;
    player->direction = UP_C;
    player->prev_direction = DOWN_C;
    player->level = 0;
    player->wins = 0;
    player->loses = 0;
    player->id = account_id;
    player->isOnline = 0;
    player->isDead = 0;
    player->name = malloc(50 * sizeof(char));
    player->pcolor = PCOLOR;
    player->psymbol = PSYMBOL;
    player->prev_y = 49;
    player->prev_x = 18 + account_id;
}
void move(map_t *map, player_t *player)
{
    int temp_x = player->x;
    int temp_y = player->y;
    if (player->direction == RIGHT_C || player->direction == RIGHT_S)
    {
        if (map->map_array[player->y][player->x + 1] == ' ' || map->map_array[player->y][player->x + 1] == 'Y')
        {
            player->x++;
        }
    }
    else if (player->direction == LEFT_C || player->direction == LEFT_S)
    {
        if (map->map_array[player->y][player->x - 1] == ' ' || map->map_array[player->y][player->x - 1] == 'Y')
        {
            player->x--;
        }
    }
    else if (player->direction == UP_C || player->direction == UP_S)
    {
        if (map->map_array[player->y - 1][player->x] == ' ' || map->map_array[player->y - 1][player->x] == 'Y')
        {
            player->y--;
        }
    }
    else if (player->direction == DOWN_C || player->direction == DOWN_S)
    {
        if (map->map_array[player->y + 1][player->x] == ' ' || map->map_array[player->y + 1][player->x] == 'Y')
        {
            player->y++;
        }
    }
    map_set(map, MAP_P_SYMBOL, temp_y, temp_x);
    map_set(map, PSYMBOL, player->y, player->x);
}

/**
 * @id refers to the id of the player of the current client
 */
void move_multi(map_t *map, player_t players[], int id)
{
    players[id].prev_x = players[id].x;
    players[id].prev_y = players[id].y;
    if (players[id].direction == RIGHT_C || players[id].direction == RIGHT_S)
    {
        if (map->map_array[players[id].y][players[id].x + 1] == ' ')
        {
            players[id].x++;
        }
    }
    else if (players[id].direction == LEFT_C || players[id].direction == LEFT_S)
    {
        if (map->map_array[players[id].y][players[id].x - 1] == ' ')
        {
            players[id].x--;
        }
    }
    else if (players[id].direction == UP_C || players[id].direction == UP_S)
    {
        if (map->map_array[players[id].y - 1][players[id].x] == ' ')
        {
            players[id].y--;
        }
    }
    else if (players[id].direction == DOWN_C || players[id].direction == DOWN_S)
    {
        if (map->map_array[players[id].y + 1][players[id].x] == ' ')
        {
            players[id].y++;
        }
    }
}

void player_die(player_t *player)
{
    player->isDead = 1;
}

void get_stats(player_t *player, monster_t monsters[], map_t *map)
{
    int i = 0;
    printf("\033[1;33m"); //Set the text to the color red
    printf("HP: %d    ", player->health);
    printf("\033[0m");

    printf("\033[1;34m");
    printf("AC: %d    ", player->accuracy);
    printf("\033[0m");

    printf("\033[1;35m");
    printf("AR: %d    ", player->armor);
    printf("\033[0m");

    printf("\033[1;36m");
    printf("AT: %d    \n", player->attack);
    printf("\033[0m");

    printf("\033[1;32m");
    printf("Wins: ");
    printf("%d", player->wins);
    printf("\033[0m");

    printf("\033[1;31m");
    printf("   Loses: ");
    printf("%d", player->loses);
    printf("\033[0m");

    printf("\033[1;37m");
    printf("  Level: ");
    printf("%d\n", map->level);
    printf("\033[0m");

    printf("Monsters left:   [");

    for (i = 0; i < map->level + 3; i++)
    {
        if (monsters[i].isDead == 0 && monsters[i].is_boss == 0)
        {
            printf("*");
        }
        else if (monsters[i].isDead == 1)
        {
            printf(" ");
        }
        else if (monsters[i].isDead == 0 && monsters[i].is_boss == 1)
        {
            redprint_char('B');
        }
    }
    printf("]\n");
    printf("Save and exit by pressing #\n");
}
void get_stats_multi(player_t players[], monster_t monsters[], map_t *map, int my_id)
{
    int i;
    for (i = 0; i < 3; i++)
    {
        if (my_id == i)
            printf("\033[1;37mMe: \033[0m ");
        printf("\033[1;33m"); //Set the text to the color red
        printf("HP: %d    ", players[i].health);
        printf("\033[0m");

        printf("\033[1;34m");
        printf("AC: %d    ", players[i].accuracy);
        printf("\033[0m");

        printf("\033[1;35m");
        printf("AR: %d    ", players[i].armor);
        printf("\033[0m");

        printf("\033[1;36m");
        printf("AT: %d  ", players[i].attack);
        printf("\033[0m");

        if(players[i].isDead == TRUE)
        {
            printf("\033[1;31m");
            printf("  *DEAD* ");
            printf("\033[0m\n");
        }
        else printf("\n");

        printf("\033[1;32m");
        printf("Wins: ");
        printf("%d", players[i].wins);
        printf("\033[0m");

        printf("\033[1;31m");
        printf("   Loses: ");
        printf("%d", players[i].loses);
        printf("\033[0m");

        printf("\033[1;37m");
        printf("  Level: ");
        printf("%d\n", map->level);
        printf("\033[0m");

        
    }
    printf("Monsters left:   [");

    for (i = 0; i < map->level + 3; i++)
    {
        if (monsters[i].isDead == 0 && monsters[i].is_boss == 0)
        {
            printf("*");
        }
        else if (monsters[i].isDead == 1)
        {
            printf(" ");
        }
        else if (monsters[i].isDead == 0 && monsters[i].is_boss == 1)
        {
            redprint_char('B');
        }
    }
    printf("]\n");
    printf("Save and exit by pressing #\n");
}
/**
 * function for miss or hit with probability based on accuracy
 * */
int attack(float accuracy, float damage, float armor)
{
    int rand_num = (rand() % (101));
    if (rand_num < accuracy)
    {
        return (((1 - (armor * 0.01)) / 2) * damage);
    }
    else
    {
        return 0;
    }
}
/**
 * adds stats to player when opened
 * */
void open_chest(chest_t chest, player_t *player)
{
    player->health += (chest.level) * 5;
    player->armor += (chest.level) * 2;
    player->attack += (chest.level) * 2;
    player->accuracy += (chest.level) * 2;
}

/**
 * checks if there is an object near the player
 * when found,if it is a monster executes attack or if it is a chest opens it
 * it doesn't consider the diagonal position
 **/
void object_found(map_t *map, player_t *player, char key_press, monster_t mons_arr[], chest_t chest_arr[])
{
    int i;
    int mons_attack, player_attack;
    for (i = 0; i < map->monsters_num; i++)
    {
        if ((mons_arr[i].y == player->y + 1 && mons_arr[i].x == player->x && (key_press == DOWN_C || key_press == DOWN_S)) ||
            (mons_arr[i].y == player->y - 1 && mons_arr[i].x == player->x && (key_press == UP_C || key_press == UP_S)) ||
            (mons_arr[i].y == player->y && mons_arr[i].x == player->x + 1 && (key_press == RIGHT_C || key_press == RIGHT_S)) ||
            (mons_arr[i].y == player->y && mons_arr[i].x == player->x - 1 && (key_press == LEFT_C || key_press == LEFT_S)))
        {
            if (mons_arr[i].isDead != TRUE)
            {
                if (player->health > 0)
                {
                    player_attack = attack((float)mons_arr[i].accuracy, (float)mons_arr[i].attack, (float)player->armor);
                    player->health -= player_attack;
                    if (player->health <= 0)
                        player_die(player); //second check for life after health so the player can t have negative hp
                }
                else
                {
                    player_die(player);
                }
                if (mons_arr[i].health > 0)
                {
                    mons_attack = attack((float)player->accuracy, (float)player->attack, (float)mons_arr[i].armor);
                    mons_arr[i].health -= mons_attack;
                }
                else
                {
                    mons_arr[i].isDead = TRUE;
                }
            }
        }
    }
    for (i = 0; i < map->chests_num; i++)
    {
        if ((chest_arr[i].y == player->y + 1 && chest_arr[i].x == player->x && (key_press == DOWN_C || key_press == DOWN_S)) ||
            (chest_arr[i].y == player->y - 1 && chest_arr[i].x == player->x && (key_press == UP_C || key_press == UP_S)) ||
            (chest_arr[i].y == player->y && chest_arr[i].x == player->x + 1 && (key_press == RIGHT_C || key_press == RIGHT_S)) ||
            (chest_arr[i].y == player->y && chest_arr[i].x == player->x - 1 && (key_press == LEFT_C || key_press == LEFT_S)))
        {
            if (chest_arr[i].isOpen != TRUE)
            {
                chest_arr[i].isOpen = TRUE;
                open_chest(chest_arr[i], player);
            }
        }
    }
}

void object_found_multi(client_t *client, map_t *map, player_t *player, char key_press, monster_t mons_arr[], chest_t chest_arr[])
{
    int i;
    char *net_buffer;
    int mons_attack, player_attack;
    for (i = 0; i < map->monsters_num; i++)
    {
        if ((mons_arr[i].y == player->y + 1 && mons_arr[i].x == player->x && (key_press == DOWN_C || key_press == DOWN_S)) ||
            (mons_arr[i].y == player->y - 1 && mons_arr[i].x == player->x && (key_press == UP_C || key_press == UP_S)) ||
            (mons_arr[i].y == player->y && mons_arr[i].x == player->x + 1 && (key_press == RIGHT_C || key_press == RIGHT_S)) ||
            (mons_arr[i].y == player->y && mons_arr[i].x == player->x - 1 && (key_press == LEFT_C || key_press == LEFT_S)))
        {
            if (mons_arr[i].isDead != TRUE)
            {
                if (player->health > 0)
                {
                    player_attack = attack((float)mons_arr[i].accuracy, (float)mons_arr[i].attack, (float)player->armor);
                    player->health -= player_attack;
                    if (player->health <= 0){
                        player_die(player); //second check for life after health so the player can t have negative hp
                        net_buffer = on_player_death(player);
                        send(client->sockfd, net_buffer, SOCK_BUFF_SZ, 0);
                        net_buffer = NULL;
                    }
                       
                }
                else
                {
                    player_die(player);
                }
                if (mons_arr[i].health > 0)
                {
                    mons_attack = attack((float)player->accuracy, (float)player->attack, (float)mons_arr[i].armor);
                    mons_arr[i].health -= mons_attack;
                }
                else
                {
                    mons_arr[i].isDead = TRUE;
                    net_buffer = on_monster_death(&mons_arr[i]);
                    send(client->sockfd, net_buffer, SOCK_BUFF_SZ, 0);
                    net_buffer = NULL;
                }
            }
        }
    }
    for (i = 0; i < map->chests_num; i++)
    {
        if ((chest_arr[i].y == player->y + 1 && chest_arr[i].x == player->x && (key_press == DOWN_C || key_press == DOWN_S)) ||
            (chest_arr[i].y == player->y - 1 && chest_arr[i].x == player->x && (key_press == UP_C || key_press == UP_S)) ||
            (chest_arr[i].y == player->y && chest_arr[i].x == player->x + 1 && (key_press == RIGHT_C || key_press == RIGHT_S)) ||
            (chest_arr[i].y == player->y && chest_arr[i].x == player->x - 1 && (key_press == LEFT_C || key_press == LEFT_S)))
        {
            if (chest_arr[i].isOpen != TRUE)
            {
                chest_arr[i].isOpen = TRUE;
                open_chest(chest_arr[i], player);
                net_buffer = on_chest_open(&chest_arr[i]);
                send(client->sockfd, net_buffer, SOCK_BUFF_SZ, 0);
                net_buffer = NULL;
            }
        }
    }
}

int check_level_up(monster_t mons_arr[], map_t *map)
{
    int i;
    for (i = 0; i < map->monsters_num; i++)
    {
        if (!mons_arr[i].isDead)
        {
            return 1;
        }
    }
    return 0;
}

void player_check_max_stats(player_t *player)
{
    if (player->armor > 100)
        player->armor = 100;
    if (player->accuracy > 100)
        player->accuracy = 100;
    if (player->attack > 100)
        player->attack = 100;
    if (player->health > 100)
        player->health = 100;
}