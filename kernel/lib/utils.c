#include "utils.h"

// Inspiré d'un code internet
// Ref : http://www.firmcodes.com/write-printf-function-c/
char *convert(unsigned int num, int base, char *buffer, unsigned int size) { 
	static char representation[]= "0123456789ABCDEF";
	
    buffer = &buffer[size-1];
	*buffer = '\0';
	
    do 
	{ 
		*--buffer = representation[num%base];
		num /= base;
	} while (num);

	return buffer;
}