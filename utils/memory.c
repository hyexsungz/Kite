#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

#include "../include/utils.h"

int kite_mkdir(const char *path)
{
#ifdef _WIN32
    return mkdir(path);
#else
    return mkdir(path, 0755);
#endif
}

int path_join(char *out, size_t size, const char *a, const char *b)
{
    if (!out || !a || !b)
        return 1;

    snprintf(out, size, "%s/%s", a, b);
    return 0;
}

int path_exists(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f)
        return 0;

    fclose(f);
    return 1;
}

int path_is_dir(const char *path)
{
    if (!path)
        return 0;

    FILE *f = fopen(path, "rb");
    if (f)
    {
        fclose(f);
        return 0;
    }

    return 1;
}