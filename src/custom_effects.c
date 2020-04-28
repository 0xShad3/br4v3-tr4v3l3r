#include "custom_effects.h"
#include <stdio.h>

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


