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

#include "custom_effects.h"
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

    buff = (char *)malloc(sizeof(int));
    recv(client->sockfd, buff, sizeof(int), 0);
    if (atoi(buff) == 1)
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
/**
 * Wait team to connect
 * On success @return 0
 * else -1
 */

int wait_team(account_t *account, client_t *client)
{
    char buff[sizeof(int)];
    recv(client->sockfd, buff, sizeof(char), 0);
    if (atoi(buff) == 1)
    {
        return 0;
    }
    return -1;
}