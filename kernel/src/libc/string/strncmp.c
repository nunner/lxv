#include "string.h"

#include "stdint.h"

int
strncmp(char *str1, char *str2, int n)
{
    uint8_t c1, c2;

    while(--n > 0)
    {
        c1 = *str1++;
        c2 = *str2++;

        if(c1 != c2)
            return c1 - c2;
        else if(c1 == '\0')
            return 0;
    }
    return 0;
}