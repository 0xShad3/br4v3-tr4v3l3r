#ifndef EVENT_HANDLER_H_
#define EVENT_HANDLER_H_

#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include "player.h"
#include "util.h"
#include "chest.h"
#include "game_logic.h"
#include "map.h"
#include "mode_handle.h"
#include "monster.h"

char *on_monster_death(monster_t *monster);
char *on_moster_update_stats(monster_t *monster);
char *on_chest_open(chest_t *chest);
char *on_player_death(player_t *player);
char *on_player_move(player_t *player);
char *on_player_update_stats(player_t *player);
#endif