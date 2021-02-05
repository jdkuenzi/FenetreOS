#include "string.h"

// Ref : Ce code source vient de la librairie string (https://code.woboq.org/linux/linux/lib/string.c.html)
// +------------------------------------------------------------+
void *memset(void *dst, int value, uint_t count) {
	char *d = dst;
	while (count--)
	{
		*d++ = value;
	}
	return dst;
}

void *memcpy(void *dst, void *src, uint_t count) {
	char *copySrc = src;
	char *copyDst = dst;
	while (count--)
	{
		*copyDst++ = *copySrc++;
	}
	return dst;
}

char *strncpy(char *dest, const char *src, uint_t count) {
	char *tmp = dest;
	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}
	return dest;
}

int strcmp(const char *p, const char *q) {
	unsigned char c1, c2;
	while (1) {
		c1 = *p++;
		c2 = *q++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}
	return 0;
	
}

int strncmp(const char *p, const char *q, uint_t count) {
	unsigned char c1, c2;
	while (count) {
		c1 = *p++;
		c2 = *q++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
		count--;
	}
	return 0;
}

int strlen(const char *s) {
	const char *sc;
	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}
// +------------------------------------------------------------+