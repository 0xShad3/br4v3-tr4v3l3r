#ifndef PLAYER_H_
#define PLAYER_H_

#define PCOLOR "[0;34m"
#define PSYMBOL 'Y'

typedef struct player_t{
    int x,y,health,armor,attack,accuracy,level,wins,loses,id,isOnline,isDead;
    char* name;
    char* pcolor ;
    char psymbol;
}player_t;
void init_default(player_t  *player);
void move(player_t *player,char input);
void die(player_t *player);
#endif