#include "map.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



/*
	initialises the map
*/
void init_map(map_t *map){
	map_constr_fn(map);
	map_parser(map);
	
	map->monsters_num = map->level + 3;
	map->chests_num = map->level;

}

/* 
	constructs the filenmae of the map file
	@format: map[N].csv
*/

void map_constr_fn(map_t* map){
	const char base[] = "map";
	char str [10];
	strcpy(map->filename,"map");
	strcat(map->filename,itoa(map->level,str,10));
	strcat(map->filename,".csv\0");
}


/* A utility function to reverse a string  */
void reverse(char str[], int length) 
{ 
	int temp;
    int start = 0; 
    int end = length -1; 
    while (start < end) 
    { 
        temp = str[start];
		str[start] = str[end];
		str[end] = temp; 
        start++; 
        end--; 
    } 
} 
  
// Implementation of itoa() 
char* itoa(int num, char* str, int base) 
{ 
    int i = 0; 
    int isNegative = 0; 
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return str; 
    } 
  
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10) 
    { 
        isNegative = 1; 
        num = -num; 
    } 
  
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    // If number is negative, append '-' 
    if (isNegative) 
        str[i++] = '-'; 
  
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    reverse(str, i); 
  
    return str; 
} 


/*
	prints the map
*/

void print_map(map_t *map){
	int i;
	int j;

	for(i=0; i < MAP_HEIGHT;i++){
		for(j=0 ; j< MAP_WIDTH-1; j++){
			printf("%c",map->map_array[i][j]);
		}
		printf("\n");
	}	
}


/*

	Parses the .csv file to the 2d array of the map_t->map_array
	@The output has to be compatible with the hardcoded directory!
		Else it will give segmentation

*/
void map_parser(map_t* map){

	char hold_buffer[101] = "./maps/";;
	int i =0;
	int j;


	strcat(hold_buffer,map->filename);
	FILE* fd = fopen(hold_buffer,"r");


	if (!fd){
		printf("An error occured, the map could not be loaded,exitting....");
		fclose(fd);
		exit(0);
	}


	for (i=0;i<MAP_HEIGHT;i++){

		fread(hold_buffer,101,sizeof(char),fd);		
		hold_buffer[101]='\0';
		//printf(hold_buffer);
		for (j=0;j<MAP_WIDTH;j++){

			if (hold_buffer[j*2] == '1'){
				map->map_array[i][j] = MAP_W_SYMBOL;

			}
			else if (hold_buffer[j*2] == '0'){
				map->map_array[i][j] = MAP_P_SYMBOL;
			}

		}
		
		map->map_array[i][MAP_WIDTH-1] = '\0';
	}

	fclose(fd);
}


