#include "stdlib.h"

#include "string.h"

char *
itoa(uint32_t value, char *str)
{
    int i = 0;
    do {
        str[i++] = value % 10 + '0';
    } while((value /= 10) > 0);

    str[i] = '\0';

    strrev(str);

    return str;
}