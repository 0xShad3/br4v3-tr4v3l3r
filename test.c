#include <stdlib.h>
#include <stdio.h>
#include "src/util.h"
#include <unistd.h>
#include <string.h>
#define SOCK_BUFF_SZ 2048
#define NET_DELIM ":"
#define SAVE_GAME_ID 55

int main(){
	
	char filename[] = "015";
	
	 char *buffer = malloc(sizeof(char) * SOCK_BUFF_SZ);
    char filename_buffer[50];
    char base[] = "./saves/multi/";
    char file_extension[] = ".rpg\0";
    char *content;
    char *md5savefile;
    size_t filesize;

    strcpy(filename_buffer, base);
    strcat(filename_buffer, filename);
    strcat(filename_buffer, file_extension);

    itoa(SAVE_GAME_ID, buffer, 10);
    strcat(buffer, NET_DELIM);

    if (access(filename_buffer, F_OK) != -1)
    {
        FILE *fd = fopen(filename_buffer, "r+");
        if (fd == NULL)
        {
            printf("THERE WAS AN ERROR");
            exit(EXIT_FAILURE);
        }
        fseek(fd, 0L, SEEK_END);
        filesize = ftell(fd);
        fseek(fd, 0L, SEEK_SET);
        content = calloc(sizeof(char), filesize);
        fread(content, sizeof(char), filesize, fd);
        printf("content %s\n\n",content);

        md5savefile = strmd5(content, strlen(content));
        printf("%s\n\n\n\n", md5savefile);
        fclose(fd);
    }
    strcat(buffer, md5savefile);
    printf("%s",buffer);
}