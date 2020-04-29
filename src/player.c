#include <stdio.h>
#include <stdlib.h>
#include "player.h"


void init_default(player_t  *player){
    player->x=1;//gia na ksekinaei apo katw aristera 
    player->y=48;
    player->health=50;
    player->armor=50;
    player->attack=50;
    player->accuracy=50;
    player->level=0;
    player->wins=0;
    player->loses=0;
    player->id=0;
    player->isOnline=0;
    player->isDead=0;
    player->name=malloc(50*sizeof(char));
    player->pcolor=PCOLOR;
    player->psymbol=PSYMBOL;
}
void move(player_t *player,char input){
    if(input == 'D') input = 'd';
	else if(input == 'S') input = 's';
	else if(input == 'W') input = 'w';
	else if(input == 'A') input = 'a';
    if(input == 'd'){
        player->x++;
    }else if(input == 'a'){
        player->x--;
    }else if(input == 'w'){
        player->y++;
    }else if(input == 's'){
        player->y--;
    }else{
        printf("wrong input");
    }
}
void die(struct player_t *player){
    player->isDead=1;
}