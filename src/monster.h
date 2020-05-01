#ifndef MONSTER_H_
#define MONSTER_H_
//#include "player.h"
#define MCOLOR "[0;31m"
#define MSYMBOL '@'

typedef struct monster_t{
    int x;
    int y;
    int health;
    int armor;
    int attack;
    int accuracy;
    int monster_type;
    int monster_id;
    int isDead;
    char msymbol;
    char* mcolor;
}monster_t;

void init_monster(monster_t *monster,int id,int x1,int y1);
void monster_die(monster_t *monster);
//void attack(monster_t *monster,player_t *player);
#endif