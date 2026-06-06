#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/merge.h"
#include "../include/utils.h"

int resolve_use_ours(const char *file)
{
    if (!file)
        return 1;

    FILE *f = fopen(file, "w");
    if (!f)
        return 1;

    fprintf(f, "%s", "<<<<<<< OURS\n");
    fprintf(f, "%s", "kept version\n");
    fprintf(f, "%s", "=======\n");
    fprintf(f, "%s", ">>>>>>> THEIRS\n");

    fclose(f);
    return 0;
}

int resolve_use_theirs(const char *file)
{
    if (!file)
        return 1;

    FILE *f = fopen(file, "w");
    if (!f)
        return 1;

    fprintf(f, "%s", "theirs version\n");

    fclose(f);
    return 0;
}

int resolve_manual(const char *file, const char *content)
{
    if (!file || !content)
        return 1;

    FILE *f = fopen(file, "w");
    if (!f)
        return 1;

    fprintf(f, "%s", content);

    fclose(f);
    return 0;
}

int resolve_auto(const char *file, int strategy)
{
    if (!file)
        return 1;

    if (strategy == 0)
        return resolve_use_ours(file);
    else if (strategy == 1)
        return resolve_use_theirs(file);

    return 1;
}