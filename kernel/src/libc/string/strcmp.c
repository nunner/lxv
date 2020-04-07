#include "string.h"

#include "stdint.h"

int
strcmp(char *str1, char *str2)
{
    while (*str1)
    {
        if(*str1 != *str2)
            break;

        str1++;
        str2++;
    }

    return *(uint8_t *) str1 - *(uint8_t *) str2;
}