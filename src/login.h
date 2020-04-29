#ifndef LOGIN_H_
#define LOGIN_H_

//Contains login and register functions
int account_exists(char *username, char *password);
int do_register(char *username,char *password);
int login_check(char *username,char *password);
int get_lastid();
void exit_choice();
void exists_error();
#endif