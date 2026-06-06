#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/utils.h"

char *str_trim(char *s)
{
    if (!s)
        return NULL;

    while (isspace((unsigned char)*s))
        s++;

    if (*s == 0)
        return s;

    char *end = s + strlen(s) - 1;

    while (end > s && isspace((unsigned char)*end))
        *end-- = 0;

    return s;
}

char *str_dup(const char *s)
{
    if (!s)
        return NULL;

    size_t len = strlen(s);

    char *copy = (char *)malloc(len + 1);
    if (!copy)
        return NULL;

    memcpy(copy, s, len + 1);
    return copy;
}

int str_startswith(const char *s, const char *prefix)
{
    if (!s || !prefix)
        return 0;

    return strncmp(s, prefix, strlen(prefix)) == 0;
}

int str_endswith(const char *s, const char *suffix)
{
    if (!s || !suffix)
        return 0;

    size_t sl = strlen(s);
    size_t su = strlen(suffix);

    if (su > sl)
        return 0;

    return strncmp(s + (sl - su), suffix, su) == 0;
}

void str_to_upper(char *s)
{
    if (!s)
        return;

    for (; *s; s++)
        *s = (char)toupper((unsigned char)*s);
}

void str_to_lower(char *s)
{
    if (!s)
        return;

    for (; *s; s++)
        *s = (char)tolower((unsigned char)*s);
}

int str_contains(const char *s, const char *sub)
{
    if (!s || !sub)
        return 0;

    return strstr(s, sub) != NULL;
}

int str_equals(const char *a, const char *b)
{
    if (!a || !b)
        return 0;

    return strcmp(a, b) == 0;
}