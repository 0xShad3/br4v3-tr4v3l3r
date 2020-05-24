#ifndef UTIL_H_
#define UTIL_H_

#define TRUE 1
#define FALSE 0

void reverse(char str[], int length);
char *itoa(int num, char *str, int base);
char* strmd5(char *str, int length);

#endif