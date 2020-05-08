#include "custom_effects.h"
#include <stdio.h>
#include <unistd.h>

//custom colored prints for text
void redprint(char *random_string){ //red text
    printf("\033[0;31m");
    printf("%s",random_string);
    printf("\033[0;0m");
}

void greenprint(char *random_string){ //green text
    printf("\033[0;32m");
    printf("%s",random_string);
    printf("\033[0;0m");
}

void orangeprint(char *random_string){ //orange text
    printf("\033[0;33m");
    printf("%s",random_string);
    printf("\033[0;0m");
}

void redprint_char(char randomchar){
    printf("\033[0;31m");
    printf("%c",randomchar);
    printf("\033[0;0m");
}

void yellowprint_char(char randomchar){
    printf("\033[0;33m");
    printf("%c",randomchar);
    printf("\033[0;0m");
}

void greenprint_char(char randomchar){ //green text
    printf("\033[0;32m");
    printf("%c",randomchar);
    printf("\033[0;0m");
}

void redprint_slow(char *message_to_print){
    int i=0;
    printf("\033[0;31m"); //set color to red
    while (message_to_print[i] != '\0')
    {

        printf("%c", message_to_print[i]);
        fflush(stdout);
        usleep(80000);
        i++;
    }
    printf("\033[0;0m"); // color reset
    // free(message_to_print);
}

void greenprint_slow(char *message_to_print){
    int i=0;
    printf("\033[0;32m"); //set color to red
    while (message_to_print[i] != '\0')
    {

        printf("%c", message_to_print[i]);
        fflush(stdout);
        usleep(80000);
        i++;
    }
    printf("\033[0;0m"); // color reset
    // free(message_to_print);
}

