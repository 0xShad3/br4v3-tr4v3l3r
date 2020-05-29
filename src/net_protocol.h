#ifndef NET_PROTOCOL_H_
#define NET_PROTOCOL_H_

#define LOGIN_ID_F 501
#define REGISTER_ID_F 502
#define EXIT_ID_F 503
#define MAX_PLAYERS 3

#include "login.h"
#include "client.h"
#include "player.h"

int login_check_multi(account_t *account, client_t *client);
int register_multi(account_t *account, client_t *client);
int wait_team(account_t *account,client_t * client);
//int set_up_players(client_t* client, player_t players[MAX_PLAYERS]);
#endif