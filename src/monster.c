#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

void init_monster(monster_t *monster, int id, int x1, int y1)
{
    monster->is_boss = 0;
    monster->attack = 100;
    monster->health = 20;
    monster->armor = 10;
    monster->accuracy = 40;
    monster->x = x1;
    monster->y = y1;
    monster->isDead = 0;
    monster->msymbol = MSYMBOL;
    monster->mcolor = MCOLOR;
    monster->monster_id = id;
}

void monster_die(monster_t monster)
{
    monster.isDead = 1;
}

void monster_boss_parser(int save_bosses[TOTAL_LVLS][2])
{
    int i, j;
    j = 0;
    i = 0;
    char *temp_map_level;
    char *temp_monster_id;
    int id, map_level;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    fp = fopen("./res/info_maps.csv", "r");
    //debug fp
    if (fp == NULL)
    {
        printf("Boss parser error!");
        exit(EXIT_FAILURE);
    }

    while (getline(&line, &len, fp) != -1)
    {
        j = 0;
        temp_map_level = strtok(line, ",");
        temp_monster_id = strtok(NULL, "\n");

        id = atoi(temp_monster_id);
        map_level = atoi(temp_map_level);

        save_bosses[i][j] = map_level;
        j++;
        save_bosses[i][j] = id;
        i++;
    }

    fclose(fp);
    free(line);
    return;
}
