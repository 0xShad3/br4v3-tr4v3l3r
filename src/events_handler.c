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

char *DELIM = ":"; //should be string to use strtok don t change it!


char *on_monster_death(monster_t *monster){
    char starting_fn_msg[4];
    char * help_sym = ":";
    char * update = malloc(sizeof(char)*3);
    char *buffer = malloc(sizeof(char)*16);

    itoa(MNSTR_DEATH_ID_M,starting_fn_msg,10);
    strcat(starting_fn_msg,":");
    strcat(buffer,starting_fn_msg);
    itoa(monster->monster_id,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    monster->isDead = 1;//force monster to die

    itoa(monster->isDead,update,10);
    strcat(buffer,update);

    return buffer;
}

char *on_moster_update_stats(monster_t *monster){
    char starting_fn_msg[4];
    char *help_sym = ":";
    char *update = malloc(sizeof(char)*3);
    char *buffer = malloc(sizeof(char)*32);
  
    itoa(MNSTR_UPDATE_ID_M,starting_fn_msg,10);
    strcat(starting_fn_msg,":");
   
    strcat(buffer,starting_fn_msg);
    
    itoa(monster->accuracy,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    itoa(monster->armor,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    itoa(monster->attack,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    itoa(monster->health,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    itoa(monster->is_boss,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    itoa(monster->isDead,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    itoa(monster->monster_id,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    itoa(monster->x,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    itoa(monster->y,update,10);
    strcat(buffer,update);
    return(buffer);
}

char *on_chest_open(chest_t *chest){
    char starting_fn_msg[4];
    char *help_sym = ":";
    char *update = malloc(sizeof(char)*3);
    char *buffer = malloc(sizeof(char)*16);
   
    itoa(CHEST_OPEN_ID_C,starting_fn_msg,10);
    strcat(starting_fn_msg,":");

    strcat(buffer,starting_fn_msg);

    itoa(chest->chest_id,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    chest->isOpen = 1;
    itoa(chest->isOpen,update,10);
    strcat(buffer,update);

    return(buffer);
}

char *on_player_death(player_t *player){
    char starting_fn_msg[4];
    char *help_sym = ":";
    char *update = malloc(sizeof(char)*3);
    char *buffer = malloc(sizeof(char)*16);
    
    itoa(PLR_DEATH_ID_P,starting_fn_msg,10);
    strcat(starting_fn_msg,":");

    strcat(buffer,starting_fn_msg);

    itoa(player->id,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    player->isDead = 1; //Force player to die
    itoa(player->isDead,update,10);
    strcat(buffer,update);

    return buffer;
}

char *on_player_move(player_t *player){
    char starting_fn_msg[4];
    char * help_sym = ":";
    char * update = malloc(sizeof(char)*3);
    char *buffer = malloc(sizeof(char)*16);
    
    itoa(PLR_MOVE_ID_P,starting_fn_msg,10);
    strcat(starting_fn_msg,":");
 
    strcat(buffer,starting_fn_msg);

    itoa(player->id,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    itoa(player->x,update,10);
    strcat(buffer,update);
    strcat(buffer,help_sym);

    itoa(player->y,update,10);
    strcat(buffer,update);

    return buffer;
}

char *on_player_update_stats(player_t *player){
    char starting_fn_msg[4];
    char *help_sym = ":";
    char *update = malloc(sizeof(char) * 3);
    char *buffer = malloc(sizeof(char)*32);
    
    itoa(PLR_UPDATE_ID_P,starting_fn_msg,10);
    strcat(starting_fn_msg,":");

    strcat(buffer,starting_fn_msg);


    itoa(player->id,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    
    itoa(player->x,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->y,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->isDead,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->loses,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->wins,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->isOnline,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->level,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->accuracy,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->armor,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->attack,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->health,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->direction,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);

    itoa(player->prev_direction,update,10);
    strcat(buffer,help_sym);
    strcat(buffer,update);
    printf("%s\n",buffer);
    return buffer;
}

void decode_on_monster_death(monster_t *monster,char *buffer_to_decode){
    char *token;
    strtok(buffer_to_decode,DELIM);
    token = strtok(NULL,DELIM); //monster id
    token = strtok(NULL,DELIM);
    //get the second number which is 1 -> force monster to die
    monster->isDead = atoi(token); //kill monster
}

void decode_on_moster_update_stats(monster_t *monster,char *buffer_to_decode){
    //[202][MON Accuracy][MON Armor][MON Attack][MON Health][MON isBoss][MON is_dead][MON monster_id][MON X][MON Y]
    char *token;
    strtok(buffer_to_decode,DELIM);

    token = strtok(NULL,DELIM);
    monster->accuracy = atoi(token);

    token = strtok(NULL,DELIM);
    monster->armor = atoi(token);

    token = strtok(NULL,DELIM);
    monster->attack = atoi(token);

    token = strtok(NULL,DELIM);
    monster->health = atoi(token);

    token = strtok(NULL,DELIM);
    monster->is_boss = atoi(token);

    token = strtok(NULL,DELIM);
    monster->isDead = atoi(token);

    token = strtok(NULL,DELIM);
    monster->monster_id = atoi(token);

    token = strtok(NULL,DELIM);
    monster->x = atoi(token);

    token = strtok(NULL,DELIM);
    monster->y = atoi(token);
}

void decode_on_player_update_stats(player_t *player,char *buffer_to_decode){
    char *token;
    strtok(buffer_to_decode,DELIM);

    token = strtok(NULL,DELIM);
    player->id = atoi(token);

    token = strtok(NULL,DELIM);
    player->x = atoi(token);

    token = strtok(NULL,DELIM);
    player->y = atoi(token);

    token = strtok(NULL,DELIM);
    player->isDead = atoi(token);
    
    token = strtok(NULL,DELIM);
    player->loses = atoi(token);

    token = strtok(NULL,DELIM);
    player->wins = atoi(token);

    token = strtok(NULL,DELIM);
    player->isOnline = atoi(token);

    token = strtok(NULL,DELIM);
    player->level = atoi(token);

    token = strtok(NULL,DELIM);
    player->accuracy = atoi(token);

    token = strtok(NULL,DELIM);
    player->armor = atoi(token);

    token = strtok(NULL,DELIM);
    player->attack = atoi(token);

    token = strtok(NULL,DELIM);
    player->health = atoi(token);

    // token = strtok(NULL,DELIM);
    // player->direction = token;

    // token = strtok(NULL,DELIM);
    // player->prev_direction = token;
}

void decode_on_player_death(player_t *player,char *buffer_to_decode){
    char *token;
    strtok(buffer_to_decode,DELIM);
    token = strtok(NULL,DELIM); //player id
    token = strtok(NULL,DELIM);
    //get the second number which is 1 -> force player to die
    player->isDead = atoi(token); //kill player
}

void decode_on_chest_open(chest_t *chest,char *buffer_to_decode){
    char *token;
    strtok(buffer_to_decode,DELIM);
    token = strtok(NULL,DELIM); //chest id
    token = strtok(NULL,DELIM);
    //get the second number which is 1 -> force chest to open
    chest->isOpen = atoi(token); //open chest
}