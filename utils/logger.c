#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/utils.h"

void *mem_alloc(size_t size)
{
    void *p = malloc(size);
    if (!p)
        return NULL;

    return p;
}

void *mem_calloc(size_t count, size_t size)
{
    void *p = calloc(count, size);
    if (!p)
        return NULL;

    return p;
}

void *mem_realloc(void *ptr, size_t size)
{
    void *p = realloc(ptr, size);
    if (!p)
        return NULL;

    return p;
}

void mem_free(void *ptr)
{
    if (ptr)
        free(ptr);
}

char *mem_strdup(const char *s)
{
    if (!s)
        return NULL;

    size_t len = strlen(s);
    char *p = malloc(len + 1);

    if (!p)
        return NULL;

    memcpy(p, s, len + 1);
    return p;
}