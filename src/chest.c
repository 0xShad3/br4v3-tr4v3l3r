#include <stdio.h>
#include <stdlib.h>
#include "chest.h"
//#include "player.h"

void init_chest(chest_t *chest, int id, int x1, int y1)
{
    chest->x = x1;
    chest->y = y1;
    chest->ccolor = CCOLOR;
    chest->csymbol = CSYMBOL;
    chest->level = 0;
    chest->isOpen = 0;
    chest->chest_level = 0;
    chest->chest_id = 0;
}
/*
void open(chest_t *chest,player_t *player){
    chest->isOpen=1;
    player->health+=(chest->level)*10;
    player->armor+=(chest->level)*10;
    player->attack+=(chest->level)*10;
    player->accuracy+=(chest->level)*10;
}
*/