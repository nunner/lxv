#include "string.h"

void *
memset(void *s, int c, int n)
{
	unsigned char* p=s;
	while(n--)
		*p++ = (unsigned char)c;
	return s;
}
