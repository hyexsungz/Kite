#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tag.h"
#include "../include/utils.h"

#define TAG_DIR ".kite/refs/tags"

int tag_create(const char *name, const char *commit, const char *message)
{
    if (!name || !commit)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", TAG_DIR, name);

    FILE *f = fopen(path, "w");
    if (!f)
        return 1;

    fprintf(f, "commit %s\n", commit);
    fprintf(f, "message %s\n", message ? message : "");

    fclose(f);
    return 0;
}

int tag_delete(const char *name)
{
    if (!name)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", TAG_DIR, name);

    return remove(path);
}

int tag_list(char ***out_names, size_t *count)
{
    (void)out_names;
    (void)count;
    return 1;
}

int tag_get(const char *name, kite_tag *out)
{
    if (!name || !out)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", TAG_DIR, name);

    FILE *f = fopen(path, "r");
    if (!f)
        return 1;

    char line[512];
    memset(out, 0, sizeof(kite_tag));

    out->name = util_strdup(name);

    while (fgets(line, sizeof(line), f))
    {
        if (strncmp(line, "commit ", 7) == 0)
            out->commit = util_strdup(line + 7);
        else if (strncmp(line, "message ", 8) == 0)
            out->message = util_strdup(line + 8);
    }

    fclose(f);
    return 0;
}

int tag_update(const char *name, const char *new_commit)
{
    if (!name || !new_commit)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", TAG_DIR, name);

    FILE *f = fopen(path, "a");
    if (!f)
        return 1;

    fprintf(f, "commit %s\n", new_commit);
    fclose(f);

    return 0;
}