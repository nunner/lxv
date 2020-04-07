#include "string.h"

void *
memset(void *s, int c, int n)
{
    for (int i = 0; i < n; ++i)
        ((int *) s)[i] = c;
    return s;
}