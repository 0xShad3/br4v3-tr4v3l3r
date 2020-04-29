#include "login.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int id=0;
int temp_id=0; //used for debuging and get id function.

/*
void player_set_id(player currentplayer){
    currentplayer.id=id;
}
this functon will be stored in player.c file
*/


int login_check(char *username,char *password){
    char *temp_username;
    char *temp_password;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    fp = fopen("./res/credentials.csv","r");
    //debug fp
    if (fp==NULL){
        printf("Login error!");
        exit(EXIT_FAILURE);
    }
    while(getline(&line, &len, fp) != -1) {
        temp_username = strtok(line," ");
        temp_password = strtok(NULL," ");
        temp_id = atoi(strtok(NULL,"\n"));
        //printf("\nTemp username is '%s' and temp password is '%s' and id is '%d'\n",temp_username,temp_password,temp_id);
        //printf("\nUsername is %s and password is %s\n",username,password);
        if(strcmp(username,temp_username)!=0 || strcmp(password,temp_password)!=0){
            continue;
        }
        else{
            fclose(fp);
            free(line);
            return 0; // if found returns 0
        }
    }
    return -1;
}


int do_register(char *username,char *password){
    FILE *fp;
    fp = fopen("./res/credentials.csv","a");
    //debug fp
    if (fp==NULL){
        printf("Register error!");
        exit(EXIT_FAILURE);
    }
    fprintf(fp,"\n%s %s %d,",username,password,get_lastid()+1); //create new line + add username/password
    fclose(fp);
    return 0;
}


int get_lastid(){
    FILE *fp;
    char *line = NULL;
    size_t len = 0;

    fp = fopen("./res/credentials.csv","r");
    //debug fp
    if (fp==NULL){
        printf("Login error!");
        exit(EXIT_FAILURE);
    }

    while(getline(&line, &len, fp) != -1){
         // Just search for the latest line, do nothing in the loop
    } 

    //split the last line to username,password,id 
    strtok(line," ");
    strtok(NULL," ");
    id = atoi(strtok(NULL,"\n"));

    fclose(fp);

    return id;
    /*
    functions returns the id found at the last line which is the last id
    Keeping these two prints for later debug if needed.
    printf("Last line %s\n", line);
    printf("Player id = %d",id);
    */
}
