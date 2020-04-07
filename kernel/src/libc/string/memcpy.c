#include "string.h"

void
memcpy (char *dest, char *src, int num)
{
    for(int i = 0; i < num; i++)
        dest[i] = src[i];
}
