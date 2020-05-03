#include <stdio.h>
#include <stdlib.h>
#include "chest.h"

void init_chest(chest_t *chest, int id, int x1, int y1)
{
    chest->x = x1;
    chest->y = y1;
    chest->ccolor = CCOLOR;
    chest->csymbol = CSYMBOL;
    chest->level = 1;
    chest->isOpen = 0;
    chest->chest_level = 0;
    chest->chest_id = 0;
}
