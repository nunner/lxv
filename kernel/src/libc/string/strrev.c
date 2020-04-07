#include "string.h"

void
strrev (char *s)
{
    int i = strlen(s) - 1, j = 0;

    char ch;
    while (i > j)
    {
        ch = s[i];
        s[i] = s[j];
        s[j] = ch;
        i--;
        j++;
    }
}