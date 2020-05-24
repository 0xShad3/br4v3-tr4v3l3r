#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>

#include "net_protocol.h"
#include "player.h"
#include "login.h"
#include "client.h"
#include "util.h"

int login_check_multi(account_t *account, client_t *client)
{

    char *message_buffer = (char *)calloc(SOCK_BUFF_SZ, sizeof(char));
    int account_len = strlen(account->username) + strlen(account->password);
    char *buff = (char *)calloc(account_len, sizeof(char));
    strcpy(buff, account->username);
    strcat(buff, account->password);
    char *account_md5 = strmd5(buff, strlen(buff));

    itoa(LOGIN_ID_F, message_buffer, 10);
    strcat(message_buffer, ":");
    strcat(message_buffer, account_md5);
    send(client->sockfd, message_buffer, SOCK_BUFF_SZ, 0);
    free(buff);
    free(account_md5);
    account_md5 = NULL;

<<<<<<< HEAD
    /* sock opt */
    gamesock->sockopt_val = setsockopt(gamesock->sockfd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char *)&so_option, sizeof(so_option)); //SO_REUSEPORT is undefined! Check later.
     if (gamesock->bind_val == -1)
=======
    buff = (char *)malloc(sizeof(int));
    recv(client->sockfd, buff, sizeof(int), 0);
    if (atoi(buff) == 1)
>>>>>>> 5f21f50617ca8954fb090d670a0d865f82d16a3d
    {
        return 0;
    }
    return -1;
}

int register_multi(account_t *account, client_t *client)
{
    char *message_buffer = (char *)calloc(SOCK_BUFF_SZ, sizeof(char));
    int account_len = strlen(account->username) + strlen(account->password);
    char *buff = (char *)calloc(account_len, sizeof(char));
    strcpy(buff, account->username);
    strcpy(buff, account->password);
    char *account_md5 = strmd5(buff, strlen(buff));
    itoa(REGISTER_ID_F, message_buffer, 10);
    strcat(message_buffer, ":");
    strcat(message_buffer, account_md5);
    send(client->sockfd, message_buffer, SOCK_BUFF_SZ, 0);
    free(buff);
    free(account_md5);
    account_md5 = NULL;

    buff = (char *)malloc(sizeof(int));
    recv(client->sockfd, buff, sizeof(int), 0);
    if (atoi(buff) == 1)
    {
        return 0;
    } 
    exists_error();
    return -1;
}