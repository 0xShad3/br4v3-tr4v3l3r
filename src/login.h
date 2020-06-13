#ifndef LOGIN_H_
#define LOGIN_H_
typedef struct account_t
{
    char username[50];
    char password[50];
    char save_file[30];
    int id;
    
}account_t;

//Contains login and register functions
int account_exists(char *username, char *password);
int do_register(account_t* login);
int login_check(account_t* login);
int get_lastid();
void exit_choice();
void exists_error();
void save_constr_fn(account_t* account);
char* save_constr_fn_multi(char* core_name);

//int login_check_multi();
#endif