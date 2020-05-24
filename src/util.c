#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

#include "util.h"

void reverse(char str[], int length)
{
	int temp;
	int start = 0;
	int end = length - 1;
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
char *itoa(int num, char *str, int base)
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
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num = num / base;
	}

	// If number is negative, append '-'
	if (isNegative)
		str[i++] = '-';

	str[i] = '\0'; // Append string terminator

	// Reverse the string
	reverse(str, i);

	return str;
}

char* strmd5(char *str, int len)
{
	unsigned char digest[16];
	MD5_CTX context;
	char* md5string = (char*)malloc(33);
	MD5_Init(&context);
	MD5_Update(&context, str, len);
	MD5_Final(digest, &context);
	
	for (int i = 0; i < 16; ++i){
		sprintf(&md5string[i * 2], "%02x", (unsigned int)digest[i]);
	}
	return md5string;
}