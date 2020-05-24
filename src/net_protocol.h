#ifndef NET_PROTOCOL_H_
#define NET_PROTOCOL_H_
#define LOGIN_ID_F 501
#define REGISTER_ID_F 502
#define EXIT_ID_F 503
#include "login.h"
#include "client.h"
int login_check_multi(account_t *account, client_t *client);
int register_multi(account_t *account, client_t *client_t);
#endif