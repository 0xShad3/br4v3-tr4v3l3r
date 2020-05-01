#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_
#include "player.h"

void init_game(int mode);
void add_stats(player_t  *player);
char key_input(char* key);
#endif