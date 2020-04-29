#ifndef LOGIN_H_
#define LOGIN_H_

//Contains login and register functions

int do_register(char *username,char *password);
int login_check(char *username,char *password);
int get_lastid();

#endif