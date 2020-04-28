#include "login.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int login_check(char *username,char *password){
    char *temp_username;
    char *temp_password;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    fp = fopen("./res/credentials.txt","r");
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
            return 0; // if found returns 0
        }
    }
    return -1;
}


int do_register(char *username,char *password){
    FILE *fp;
    fp = fopen("./res/credentials.txt","a");
    //debug fp
    if (fp==NULL){
        printf("Register error!");
        exit(EXIT_FAILURE);
    }
    fprintf(fp,"\n%s %s",username,password); //create new line + add username/password
    fclose(fp);
    return 0;
}
