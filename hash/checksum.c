#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/hash.h"

int checksum_data(const void *data, size_t size, char *out, size_t out_size)
{
    if (!data || !out)
        return 1;

    unsigned long sum = 0;
    const unsigned char *p = (const unsigned char *)data;

    for (size_t i = 0; i < size; i++)
        sum = (sum * 31) + p[i];

    snprintf(out, out_size, "%lx", sum);

    return 0;
}

int checksum_file(const char *path, char *out, size_t out_size)
{
    if (!path || !out)
        return 1;

    FILE *f = fopen(path, "rb");
    if (!f)
        return 1;

    unsigned long sum = 0;
    int c;

    while ((c = fgetc(f)) != EOF)
        sum = (sum * 31) + (unsigned char)c;

    fclose(f);

    snprintf(out, out_size, "%lx", sum);

    return 0;
}