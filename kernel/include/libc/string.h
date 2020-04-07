#ifndef LEOS_STRING_H
#define LEOS_STRING_H

void
memcpy (char *dest, char *src, int num);

int
strlen (char *s);

void
strrev (char *s);

char *
strcat (char *s1, char *s2);

int
strcmp(char *str1, char *str2);

int
strncmp(char *str1, char *str2, int n);

void *
memset(void *s, int c, int n);

#endif //LEOS_STRING_H
