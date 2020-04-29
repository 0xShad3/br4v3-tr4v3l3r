#ifndef MONSTER_H_
#define MONSTER_H_
//#include "player.h"
#define MCOLOR "[0;31m"
#define MSYMBOL '@'

typedef struct monster_t{
    int x,y,health,armor,attack,accuracy,monster_type,monster_id,isDead;
    char msymbol;
    char* mcolor;
}monster_t;
void init_monster(monster_t *monster,int x1,int y1);
//void die(monster_t *monster); 
//void attack(monster_t *monster,player_t *player);
#endif