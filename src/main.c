#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "login.h"
#include "custom_effects.h"
#include "game_logic.h"


#define SINGLE_PLR 0
#define MULTI_PLR 1

char welcome_msg[] = "Hello stranger..\n";
char wrong_creds[] = "[-] Wrong username or password. Exiting....";
char waiting_game[] = "[+] Please wait while the game is loading...";
char username[50];
char password[50];

int main(int argc, char *argv[])
{
    int i = 0;
    int selection; //used for all main menu selections
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

    printf("Select a mode:\n[1] Single-Player\n[2] Multi-Player\n");
    scanf("%d", &selection);
    while (selection != 2 && selection != 1)
    {
        printf("\033[0;31m");
        printf("[-] Try again..\n");
        printf("\033[0;0m");
        printf("Select a mode:\n[1] Single-Player\n[2] Multi-Player\n");
        scanf("%d", &selection);
    }

    //Single player mode checks
    if (selection == 1)
    {
        selection = 0;
        printf("[1] Login\n[2] Register\n");
        scanf("%d", &selection);
        while (selection != 2 && selection != 1)
        {
            printf("\033[0;31m");
            printf("[-] Try again..\n");
            printf("\033[0;0m");
            printf("[1] Login\n[2] Register\n");
            scanf("%d", &selection);
        }
        //login action check
        if (selection == 1)
        {
            greenprint("[+]Enter your username and password:\nUsername: ");
            scanf("%s", username);

            greenprint("Password: ");
            scanf("%s", password);

            if (login_check(username, password) != 0)
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

            //login success
            printf("Hello %s. \n", username);
            i = 0;
            while (wrong_creds[i] != '\0')
            {
                printf("%c", waiting_game[i]);
                fflush(stdout);
                usleep(70000);
                i++;
            }
            printf("\n");
            
            // Starting game on single player mode
            init_game(SINGLE_PLR);
        }

        if (selection == 2)
        {
            orangeprint("[+]Enter your player`s username and password:\n");
            greenprint("Username: ");
            scanf("%s", username);
            greenprint("Password: ");
            scanf("%s", password);
            if (strlen(username) != 0 && strlen(password) != 0)
            {
                if (do_register(username, password) != 0)
                {
                    redprint("[-] Fatal error at registring. Try later");
                }
            }
            else
            {
                redprint("What are you trying to do...");
            }
        }
    }

    if (selection == 2)
    {
        //TODO add multiplayer code
    }
}
