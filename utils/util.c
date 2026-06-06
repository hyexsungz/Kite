#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/utils.h"

char *util_strdup(const char *s)
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

int util_read_file(const char *path, char **out_data, size_t *out_size)
{
    if (!path || !out_data)
        return 1;

    FILE *f = fopen(path, "rb");
    if (!f)
        return 1;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    if (size < 0)
    {
        fclose(f);
        return 1;
    }

    fseek(f, 0, SEEK_SET);

    char *buf = (char *)malloc((size_t)size + 1);
    if (!buf)
    {
        fclose(f);
        return 1;
    }

    size_t read_size = fread(buf, 1, (size_t)size, f);
    buf[read_size] = '\0';

    fclose(f);

    *out_data = buf;

    if (out_size)
        *out_size = read_size;

    return 0;
}

int util_write_file(const char *path, const void *data, size_t size)
{
    if (!path || (!data && size > 0))
        return 1;

    FILE *f = fopen(path, "wb");
    if (!f)
        return 1;

    size_t written = fwrite(data, 1, size, f);
    fclose(f);

    return (written == size) ? 0 : 1;
}

int util_file_exists(const char *path)
{
    if (!path)
        return 0;

    FILE *f = fopen(path, "rb");
    if (!f)
        return 0;

    fclose(f);
    return 1;
}

int util_file_delete(const char *path)
{
    if (!path)
        return 1;

    return remove(path);
}