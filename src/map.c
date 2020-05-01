#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "map.h"
#include "chest.h"
#include "monster.h"
#include "util.h"
#include "custom_effects.h"


/*
	initialises the map
*/
void init_map(map_t *map,monster_t mons_arr[],chest_t chest_arr[])
{
	map->monsters_num = map->level + 3;
	map->chests_num = map->level;
	map_constr_fn(map);
	map_parser(map,mons_arr,chest_arr);

	
}

/* 
	constructs the filenmae of the map file
	@format: map[N].csv
*/

void map_constr_fn(map_t *map)
{
	const char base[] = "map";
	char str[10];
	strcpy(map->filename, "map");
	strcat(map->filename, itoa(map->level, str, 10));
	strcat(map->filename, ".csv\0");
}

/*
	prints the map
*/

void print_map(map_t *map)
{
	int i;
	int j;

	for (i = 0; i < MAP_HEIGHT; i++)
	{
		for (j = 0; j < MAP_WIDTH - 1; j++)
		{
			if(map->map_array[i][j] == '@'){
				redprint_char(map->map_array[i][j]);
			}

			else if(map->map_array[i][j] == '$'){
				yellowprint_char(map->map_array[i][j]);
			}

			else if(map->map_array[i][j] == 'Y'){
				greenprint_char(map->map_array[i][j]);
			}

			else{
				printf("%c",map->map_array[i][j]);
			}

		}
		printf("\n");
	}
}

/*

	Parses the .csv file to the 2d array of the map_t->map_array
	@The output has to be compatible with the hardcoded directory!
		Else it will give segmentation

*/
void map_parser(map_t *map,monster_t mons_arr[],chest_t chest_arr[])
{

	char hold_buffer[101] = "./maps/";
	int monster_counter = 0;
	int chest_counter = 0;
	int mons_id;
	int chest_id;
	int i = 0;
	int j;
	/*
		Over here hold buffer is used to construct the file name of
		the csv map file
	
	*/
	strcat(hold_buffer, map->filename);
	FILE *fd = fopen(hold_buffer, "r");
	/*
		Debug for error
	*/
	if (fd == NULL)
	{
		printf("An error occured, the map could not be loaded,exitting....");
		fclose(fd);
		exit(0);
	}

	for (i = 0; i < MAP_HEIGHT; i++)
	{

		fread(hold_buffer, 101, sizeof(char), fd);
		hold_buffer[101] = '\0';
		//printf(hold_buffer);
		for (j = 0; j < MAP_WIDTH; j++)
		{
			/*
				Hold buffer skips the comma chars 
				parses the map to 2d map array 
			*/
		
			if (hold_buffer[j * 2] == '0')
			{
				map->map_array[i][j] = MAP_P_SYMBOL;
			}
			else if (hold_buffer[j * 2] == '1')
			{
				map->map_array[i][j] = MAP_W_SYMBOL;
			}
			else if (hold_buffer[j * 2] == '2')
			{
				map->map_array[i][j] = CSYMBOL;
				chest_id = map->chests_num - (map->chests_num - chest_counter);
				chest_arr[chest_counter].chest_id = chest_id;
				chest_arr[chest_counter].x = i;
				chest_arr[chest_counter].y = j;
				chest_counter++;
			}
			else if (hold_buffer[j * 2] == '3')
			{
				map->map_array[i][j] = MSYMBOL;
				mons_id = map->monsters_num - (map->monsters_num - monster_counter);
				mons_arr[monster_counter].monster_id = mons_id;
				mons_arr[monster_counter].x = j;
				mons_arr[monster_counter].y = i;
				monster_counter++;
			}
		}

		map->map_array[i][MAP_WIDTH - 1] = '\0';
	}

	fclose(fd);
}

/*
	On error return 1;
	Generic function to set an array symbol;
*/
int map_set(map_t *map, char symbol, int x, int y)
{
	if (x == 0 || y == 0 || y == MAP_HEIGHT - 1 || x == MAP_WIDTH - 1)
	{
		return 1;
	}
	else
	{
		map->map_array[y][x] = symbol;
		return 0;
	}
}
