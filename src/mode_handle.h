#ifndef MODE_HANDLE_H_
#define MODE_HANDLE_H_

#include "login.h"
#include "client.h"
void init_game_single(account_t *account,int mode);
void init_game_multi(account_t *account, client_t *client);
#endif