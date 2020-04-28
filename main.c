#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int login_check(char *username,char *password);
int do_register(char *username,char *password);
void greenprint(char *random_string);
void orangeprint(char *random_string);
void redprint(char *random_string);

char welcome_msg[] = "Hello stranger..\n";
char wrong_creds[] = "[-] Wrong username or password. Exiting....";
char waiting_game[] = "[+] Please wait while the game is loading...";
char username[50];
char password[50];

int main(int argc,char *argv[]){
    int i=0;
    int selection; //used for all main menu selections
    if(argc!=1){
        redprint("Nope :( . You are not supposed to use arguments");
        exit(0);
    }


    //typing effect for terminal
    printf("\033[0;31m"); //set color to red
    while(welcome_msg[i]!='\0'){
        
        printf("%c",welcome_msg[i]);
        fflush(stdout);
        usleep(80000);
        i++;
    }
    printf("\033[0;0m"); // color reset

    
    printf("Select a mode:\n[1] Single-Player\n[2] Multi-Player\n");
    scanf("%d",&selection);
    while(selection!=2 && selection!=1){
        printf("\033[0;31m");
        printf("[-] Try again..\n");
        printf("\033[0;0m");
        printf("Select a mode:\n[1] Single-Player\n[2] Multi-Player\n");
        scanf("%d",&selection);
        }
    

    //Single player mode checks
    if(selection==1){
        selection=0;
        printf("[1] Login\n[2] Register\n");
        scanf("%d",&selection);
        while(selection!=2 && selection!=1){
            printf("\033[0;31m");
            printf("[-] Try again..\n");
            printf("\033[0;0m");
            printf("[1] Login\n[2] Register\n");
            scanf("%d",&selection);
        }
        //login action check
        if(selection == 1){
            greenprint("[+]Enter your username and password:\nUsername: ");
            scanf("%s",username);

            greenprint("Password: ");
            scanf("%s",password);


            if(login_check(username,password)!=0){
                i=0;
                printf("\033[0;31m"); //set color to red
                while(wrong_creds[i]!='\0'){
                    printf("%c",wrong_creds[i]);
                    fflush(stdout);
                    usleep(70000);
                    i++;
                }
                printf("\033[0;0m\n"); // color reset
                return 0;
            }

            //login success
            printf("Hello %s. \n",username);
            i=0;
            while(wrong_creds[i]!='\0'){
                    printf("%c",waiting_game[i]);
                    fflush(stdout);
                    usleep(70000);
                    i++;
                }
            printf("\n");

        }

        if(selection == 2){
            orangeprint("[+]Enter your player`s username and password:\n");
            greenprint("Username: ");
            scanf("%s",username);
            greenprint("Password: ");
            scanf("%s",password);
            if(strlen(username)!=0 && strlen(password)!=0){
                if(do_register(username,password)!=0){
                    redprint("[-] Fatal error at registring. Try later");
                }
            }
            else{
                redprint("What are you trying to do...");
            }
        }
    }

    if(selection==2){
        //TODO add multiplayer code
    }
}

int login_check(char *username,char *password){
    char *temp_username;
    char *temp_password;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    fp = fopen("credentials.txt","r");
    //debug fp
    if (fp==NULL){
        printf("Login error!");
        exit(EXIT_FAILURE);
    }
    while(getline(&line, &len, fp) != -1) {
        temp_username = strtok(line," ");
        temp_password = strtok(NULL,"\n");
        printf("\nTemp username is '%s' and temp password is '%s'\n",temp_username,temp_password);
        printf("\nUsername is %s and password is %s\n",username,password);
        if(strcmp(username,temp_username)!=0 || strcmp(password,temp_password)!=0){
            continue;
        }
        else{
            fclose(fp);
            free(line);
            return 0;
        }
    }
    return -1;
}

void redprint(char *random_string){
    printf("\033[0;31m");
    printf("%s",random_string);
    printf("\033[0;0m");
}

void greenprint(char *random_string){
    printf("\033[0;32m");
    printf("%s",random_string);
    printf("\033[0;0m");
}

void orangeprint(char *random_string){
    printf("\033[0;33m");
    printf("%s",random_string);
    printf("\033[0;0m");
}

int do_register(char *username,char *password){
    FILE *fp;
    fp = fopen("credentials.txt","a");
    //debug fp
    if (fp==NULL){
        printf("Register error!");
        exit(EXIT_FAILURE);
    }
    fprintf(fp,"\n%s %s",username,password); //create new line + add username/password
    fclose(fp);
    return 0;
}