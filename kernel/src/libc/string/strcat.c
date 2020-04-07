#include "string.h"

char *
strcat (char *s1, char *s2)
{
    char *dest = s1 + strlen(s1);

    while(*s2 != '\0')
        *dest++ = *s2++;

    *dest = '\0';

    return s1;
}