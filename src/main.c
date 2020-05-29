#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "login.h"
#include "custom_effects.h"
#include "mode_handle.h"

#include "net_protocol.h"
#include "client.h"

#define SINGLE_PLR 0
#define MULTI_PLR 1

char welcome_msg[] = "Hello stranger..\n";
char wrong_creds[] = "[-] Wrong username or password. Exiting....";
char waiting_game[] = "[+] Please wait while the game is loading...";

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int i = 0;
    int selection = 0;
    account_t account;
    client_t client;
    //used for all main menu selections
    if (argc != 1)
    {
        redprint("Nope :( . You are not supposed to use arguments");
        exit(0);
    }
    //typing effect for terminal
    printf("\033[0;31m"); //set color to red
    while (welcome_msg[i] != '\0')
    {

        printf("%c", welcome_msg[i]);
        fflush(stdout);
        usleep(80000);
        i++;
    }
    printf("\033[0;0m"); // color reset

    printf("Select a mode:\n[1] Single-Player\n[2] Multi-Player\n[3] Exit\n");
    scanf("%d", &selection);
    while (selection != 2 && selection != 1 && selection != 3)
    {
        printf("\033[0;31m");
        printf("[-] Try again..\n");
        printf("\033[0;0m");
        printf("Select a mode:\n[1] Single-Player\n[2] Multi-Player\n[3] Exit\n");
        scanf("%d", &selection);
    }

    //Single player mode checks
    if (selection == 1)
    {
        selection = 0;
        printf("[1] Login\n[2] Register and play\n[3] Exit\n");
        scanf("%d", &selection);
        while (selection != 2 && selection != 1 && selection != 3)
        {
            printf("\033[0;31m");
            printf("[-] Try again..\n");
            printf("\033[0;0m");
            printf("[1] Login\n[2] Register and play\n");
            scanf("%d", &selection);
        }
        //login action check
        if (selection == 1)
        {
            greenprint("[+]Enter your username and password:\nUsername: ");
            scanf("%s", account.username);

            greenprint("Password: ");
            scanf("%s", account.password);

            if (login_check(&account) != 0)
            {
                i = 0;
                printf("\033[0;31m"); //set color to red
                while (wrong_creds[i] != '\0')
                {
                    printf("%c", wrong_creds[i]);
                    fflush(stdout);
                    usleep(70000);
                    i++;
                }
                printf("\033[0;0m\n"); // color reset
                return 0;
            }

            printf("Hello %s. \n", account.username);
            int i = 0;
            while (waiting_game[i] != '\0')
            {
                printf("%c", waiting_game[i]);
                fflush(stdout);
                usleep(70000);
                i++;
            }
            printf("\n");

            // Construct the file path
            save_constr_fn(&account);
            // Starting game on single player mode
            init_game_single(&account,1);
        }

        if (selection == 2)
        {
            orangeprint("[+]Enter your player`s username and password:\n");
            greenprint("Username: ");
            scanf("%s", account.username);
            greenprint("Password: ");
            scanf("%s", account.password);
            if (!strlen(account.username) || !strlen(account.password))
            {
                redprint("What are you trying to do...");
            }
            else
            {
                do_register(&account);
                printf("Hello %s. \n", account.username);
                i = 0;
                while (waiting_game[i] != '\0')
                {
                    printf("%c", waiting_game[i]);
                    fflush(stdout);
                    usleep(70000);
                    i++;
                }

                // Construct the file path
                save_constr_fn(&account);
                // Starting game on single player mode
                init_game_single(&account,1);
            }
        }
    }
    if (selection == 2)
    {
        selection = 0;
        printf("Multiplayer mode!\n[1] Login\n[2] Register and play\n[3] Exit\n");
        scanf("%d", &selection);
        while (selection != 2 && selection != 1 && selection != 3)
        {
            printf("\033[0;31m");
            printf("[-] Try again..\n");
            printf("\033[0;0m");
            printf("Multiplayer mode!\n[1] Login\n[2] Register and play\n");
            scanf("%d", &selection);
        }
        //login action check
        if (selection == 1)
        {
            /**
             **Connection to server starts
             */
            if (!connect_server(&client))
            {
                return EXIT_SUCCESS;
            }
            greenprint("[+]Enter your username and password:\nUsername: ");
            scanf("%s", account.username);

            greenprint("Password: ");
            scanf("%s", account.password);
            printf("username: %s| pw : %s|\n", account.username, account.password);
            if (login_check_multi(&account, &client) != 0)
            {
                i = 0;
                printf("\033[0;31m"); //set color to red
                while (wrong_creds[i] != '\0')
                {
                    printf("%c", wrong_creds[i]);
                    fflush(stdout);
                    usleep(70000);
                    i++;
                }
                printf("\033[0;0m\n"); // color reset
                return 0;
            }
            printf("Hello %s. \n", account.username);

            greenprint("Waiting for all the players to join the game ...");
            if (!wait_team(&account, &client))
            {
                init_game_multi(&account, &client);
            }

            // Construct the file path
            //save_constr_fn(&account);
            // Starting game on single player mode
        }

        if (selection == 2)
        {
            if (!connect_server(&client))
            {
                return EXIT_SUCCESS;
            }
            orangeprint("[+]Enter your player`s username and password:\n");
            greenprint("Username: ");
            scanf("%s", account.username);
            greenprint("Password: ");
            scanf("%s", account.password);
            if (!strlen(account.username) || !strlen(account.password))
            {
                redprint("What are you trying to do...");
            }
            else
            {
                if (!register_multi(&account, &client))
                {
                    printf("Hello %s. \n", account.username);
                    greenprint("Waiting for all the players to join the game ...");
                    if (!wait_team(&account, &client))
                    {
                        init_game_multi(&account, &client);
                    }
                }
            }
        }

        if (selection == 3)
        {
            exit_choice();
        }
        return EXIT_SUCCESS;
    }
}