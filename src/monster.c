#include <stdio.h>
#include <stdlib.h>
#include "monster.h"

void init_monster(monster_t *monster, int x1, int y1)
{
    monster->monster_type = 0;
    monster->attack = 10;
    monster->health = 10;
    monster->armor = 10;
    monster->accuracy = 10;
    monster->x = x1;
    monster->y = y1;
    monster->isDead = 0;
    monster->msymbol = MSYMBOL;
    monster->mcolor = MCOLOR;
    monster->monster_id = 0;
}