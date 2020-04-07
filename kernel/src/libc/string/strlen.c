#include "string.h"

int strlen(char *s)
{
    int i = 0;
    while(s[i++] != '\0')
        ;
    return i - 1;
}