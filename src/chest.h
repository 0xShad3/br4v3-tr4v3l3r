#ifndef CHEST_H_
#define CHEST_H_



#define CCOLOR "[0;33m"
#define CSYMBOL '$'
typedef struct chest_t{
    int x,y,level,isOpen,chest_id,chest_level;
    char csymbol;
    char* ccolor;
}chest_t;
void init_chest(chest_t *chest,int id,int x1,int y1);

#endif