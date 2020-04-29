#ifndef CHEST_H_
#define CHEST_H_

//#include "player.h"


#define CCOLOR "[0;33m"
#define CSYMBOL 'C'
typedef struct chest_t{
    int x,y,level,isOpen,chest_id,chest_level;
    char csymbol;
    char* ccolor;
}chest_t;
void init_chest(chest_t *chest,int x1,int y1);
//void open(chest_t *chest,player_t *player);
#endif