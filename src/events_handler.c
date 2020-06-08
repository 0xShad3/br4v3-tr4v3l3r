#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "player.h"
#include "util.h"
#include "chest.h"
#include "game_logic.h"
#include "map.h"
#include "mode_handle.h"
#include "monster.h"
#include "events_handler.h"

char *on_monster_death(monster_t *monster)
{
    char starting_fn_msg[4];
    char *help_sym = ":";
    char *update = malloc(sizeof(char) * 3);
    char *buffer = malloc(sizeof(char) * 16);

    itoa(MNSTR_DEATH_ID_M, starting_fn_msg, 10);
    strcat(starting_fn_msg, ":");
    strcat(buffer, starting_fn_msg);


    itoa(monster->monster_id, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    monster->isDead = 1; //force monster to die

    itoa(monster->isDead, update, 10);
    strcat(buffer, update);

    return buffer;
}

char *on_moster_update_stats(monster_t *monster)
{
    char starting_fn_msg[4];
    char *help_sym = ":";
    char *update = malloc(sizeof(char) * 3);
    char *buffer = malloc(sizeof(char) * 32);

    itoa(MNSTR_UPDATE_ID_M, starting_fn_msg, 10);
    strcat(starting_fn_msg, ":");

    strcat(buffer, starting_fn_msg);

    itoa(monster->monster_id, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    itoa(monster->armor, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    itoa(monster->attack, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    itoa(monster->health, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    itoa(monster->is_boss, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    itoa(monster->isDead, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    itoa(monster->accuracy, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    itoa(monster->x, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    itoa(monster->y, update, 10);
    strcat(buffer, update);
    return (buffer);
}

char *on_chest_open(chest_t *chest)
{
    char starting_fn_msg[4];
    char *help_sym = ":";
    char *update = malloc(sizeof(char) * 3);
    char *buffer = malloc(sizeof(char) * 16);

    itoa(CHEST_OPEN_ID_C, starting_fn_msg, 10);
    strcat(starting_fn_msg, ":");

    strcat(buffer, starting_fn_msg);

    itoa(chest->chest_id, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    chest->isOpen = 1;
    itoa(chest->isOpen, update, 10);
    strcat(buffer, update);

    return (buffer);
}

char *on_player_death(player_t *player)
{
    char starting_fn_msg[4];
    char *help_sym = ":";
    char *update = malloc(sizeof(char) * 3);
    char *buffer = malloc(sizeof(char) * 16);

    itoa(PLR_DEATH_ID_P, starting_fn_msg, 10);
    strcat(starting_fn_msg, ":");

    strcat(buffer, starting_fn_msg);

    itoa(player->id, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    player->isDead = 1; //Force player to die
    itoa(player->isDead, update, 10);
    strcat(buffer, update);

    return buffer;
}

char *on_player_move(player_t *player)
{
    char starting_fn_msg[4];
    char *help_sym = ":";
    char *update = malloc(sizeof(char) * 3);
    char *buffer = malloc(sizeof(char) * SOCK_BUFF_SZ);

    itoa(PLR_MOVE_ID_P, starting_fn_msg, 10);
    strcat(starting_fn_msg, ":");

    strcat(buffer, starting_fn_msg);

    itoa(player->id, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    itoa(player->x, update, 10);
    strcat(buffer, update);
    strcat(buffer, help_sym);

    itoa(player->y, update, 10);
    strcat(buffer, update);

    return buffer;
}

char *on_player_update_stats(player_t *player)
{
    char starting_fn_msg[4];
    char *help_sym = ":";
    char *update = malloc(sizeof(char) * 3);
    char *buffer = malloc(sizeof(char) * SOCK_BUFF_SZ);

    itoa(PLR_UPDATE_ID_P, starting_fn_msg, 10);

    strcat(buffer, starting_fn_msg);

    itoa(player->id, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->x, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->y, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->isDead, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->loses, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->wins, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->isOnline, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->level, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->accuracy, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->armor, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->attack, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->health, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->direction, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);

    itoa(player->prev_direction, update, 10);
    strcat(buffer, help_sym);
    strcat(buffer, update);
    printf("%s\n", buffer);

    buffer[strlen(buffer)] = '\0';
    return buffer;
}

int decode_on_monster_death(monster_t mons_arr[], char *buffer_to_decode, map_t *map)
{
    int i;
    char *token;
    strtok(buffer_to_decode, NET_DELIM);
    token = strtok(NULL, NET_DELIM); //monster id
    for(i=0; i<map->monsters_num; i++){
        if(mons_arr[i].monster_id == atoi(token)){
            token = strtok(NULL, NET_DELIM); //get the second number which is 1 -> force monster to die
            mons_arr[i].isDead = atoi(token);
            break;
        }
    }
    return 0;
}

int decode_on_monster_update_stats(monster_t mons_arr[], char *buffer_to_decode, map_t *map)
{
    //[202][MON monster_id][MON Armor][MON Attack][MON Health][MON isBoss][MON is_dead][MON Accuracy][MON X][MON Y]
    char *token;
    int i;
    strtok(buffer_to_decode, NET_DELIM);

    token = strtok(NULL, NET_DELIM);
    
    for(i=0; i<map->monsters_num; i++){
        if(mons_arr[i].monster_id == atoi(token)){
            token = strtok(NULL, NET_DELIM);
            mons_arr[i].armor = atoi(token);

            token = strtok(NULL, NET_DELIM);
            mons_arr[i].attack = atoi(token);

            token = strtok(NULL, NET_DELIM);
            mons_arr[i].health = atoi(token);

            token = strtok(NULL, NET_DELIM);
            mons_arr[i].is_boss = atoi(token);

            token = strtok(NULL, NET_DELIM);
            mons_arr[i].isDead = atoi(token);

            token = strtok(NULL, NET_DELIM);
            mons_arr[i].accuracy = atoi(token);

            token = strtok(NULL, NET_DELIM);
            mons_arr[i].x = atoi(token);

            token = strtok(NULL, NET_DELIM);
            mons_arr[i].y = atoi(token);
            break;
        }
    }
    return 0;
}

int decode_on_player_update_stats(player_t players_arr[], char *buffer_to_decode)
{
    char *token;
    int i;
    strtok(buffer_to_decode, NET_DELIM);

    token = strtok(NULL, NET_DELIM); //player id
    for(i=0; i<3; i++){
        if(players_arr[i].id == atoi(token)){
            token = strtok(NULL, NET_DELIM);
            players_arr[i].x = atoi(token);

            token = strtok(NULL, NET_DELIM);
            players_arr[i].y = atoi(token);

            token = strtok(NULL, NET_DELIM);
            players_arr[i].isDead = atoi(token);

            token = strtok(NULL, NET_DELIM);
            players_arr[i].loses = atoi(token);

            token = strtok(NULL, NET_DELIM);
            players_arr[i].wins = atoi(token);

            token = strtok(NULL, NET_DELIM);
            players_arr[i].isOnline = atoi(token);

            token = strtok(NULL, NET_DELIM);
            players_arr[i].level = atoi(token);

            token = strtok(NULL, NET_DELIM);
            players_arr[i].accuracy = atoi(token);

            token = strtok(NULL, NET_DELIM);
            players_arr[i].armor = atoi(token);

            token = strtok(NULL, NET_DELIM);
            players_arr[i].attack = atoi(token);

            token = strtok(NULL, NET_DELIM);
            players_arr[i].health = atoi(token);
            break;
        }
    }
    // token = strtok(NULL,NET_DELIM);
    // player->direction = token;

    // token = strtok(NULL,NET_DELIM);
    // player->prev_direction = token;
    return 0;
}

int decode_on_player_death(player_t players_arr[], char *buffer_to_decode)
{
    char *token;
    int i;
    strtok(buffer_to_decode, NET_DELIM);
    token = strtok(NULL, NET_DELIM); //player id
    // id = token;
    // token = strtok(NULL, NET_DELIM);
    for(i=0; i< 3; i++){
        if(players_arr[i].id == atoi(token)){
            token = strtok(NULL, NET_DELIM);
            //get the second number which is 1 -> force player to die
            players_arr[i].isDead = atoi(token); //kill player
            break;
        }
    }
    return 0;
}

int decode_on_chest_open(chest_t chests_arr[], char *buffer_to_decode, map_t *map)
{
    char *token;
    int i;
    strtok(buffer_to_decode, NET_DELIM);
    token = strtok(NULL, NET_DELIM); //chest id

    for(i=0; i<map->level; i++){ //map level cause chests number = map level
        if(chests_arr[i].chest_id == atoi(token)){
            token = strtok(NULL, NET_DELIM);
            //get the second number which is 1 -> force chest to open
            chests_arr[i].isOpen = atoi(token); //open chest
            break;
        }
    }
    return 0;
}

int decode_on_player_move(player_t players_arr[], char *buffer_to_decode)
{   
    char *token;
    int i;
    strtok(buffer_to_decode, NET_DELIM);

    token = strtok(NULL, NET_DELIM); //player id
    for(i=0;i<3;i++){
        if(players_arr[i].id == atoi(token)){
            token = strtok(NULL, NET_DELIM);
            players_arr[i].x = atoi(token);

            token = strtok(NULL, NET_DELIM);
            players_arr[i].y = atoi(token);
            break;
        }
    }
    return 0;
}

int decode_on_map_receive(map_t *map, char *buffer_to_decode)
{
    char *token;
    char hold_buffer[101];
    char content[5050];
    strtok(buffer_to_decode, NET_DELIM);

    token = strtok(NULL, NET_DELIM);
    map->level = atoi(token);

    token = strtok(NULL, NET_DELIM);
    char *md5_map = token;
    md5_map[32] = '\0';

    map_constr_fn(map);
    strcpy(hold_buffer, "./maps/");
    strcat(hold_buffer, map->filename);
    FILE *fd = fopen(hold_buffer, "r");
    if (fd == NULL)
    {
        fprintf(stderr, "An error occured, the map could not be loaded,exitting....");
        fclose(fd);
        exit(EXIT_FAILURE);
    }
    bzero(content, 5050);
    fread(content, sizeof(content), sizeof(char), fd);

    char *content_md5 = strmd5(content, strlen(content));
    if (!strcmp(content_md5, md5_map))
    {
        fclose(fd);
        return 1;
    }

    fprintf(stderr, "The map file is corrupted or modified!");
    fclose(fd);
    free(content_md5);
    free(md5_map);
    return 0;
}