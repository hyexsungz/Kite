#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/commit.h"
#include "../include/utils.h"
#include "../include/hash.h"
#include "../include/object.h"
#include "../include/index.h"

int commit_create(const char *message, char *out_id, size_t size)
{
    if (!message || !out_id)
        return 1;

    kite_hash h;
    char seed[512];

    snprintf(seed, sizeof(seed), "%s-%ld", message, (long)time(NULL));

    hash_string(seed, &h);

    snprintf(out_id, size, "%s", h.value);

    return 0;
}

int commit_write(const kite_commit *commit)
{
    if (!commit || !commit->id)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", commit->id);

    FILE *f = fopen(path, "w");
    if (!f)
        return 1;

    fprintf(f, "id %s\n", commit->id);
    fprintf(f, "message %s\n", commit->message ? commit->message : "");
    fprintf(f, "parent %s\n", commit->parent ? commit->parent : "");
    fprintf(f, "tree %s\n", commit->tree ? commit->tree : "");
    fprintf(f, "timestamp %ld\n", commit->timestamp);

    fclose(f);
    return 0;
}

int commit_read(const char *id, kite_commit *out)
{
    if (!id || !out)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", id);

    FILE *f = fopen(path, "r");
    if (!f)
        return 1;

    char line[512];

    memset(out, 0, sizeof(kite_commit));

    out->id = util_strdup(id);

    while (fgets(line, sizeof(line), f))
    {
        if (strncmp(line, "message ", 8) == 0)
            out->message = util_strdup(line + 8);
        else if (strncmp(line, "parent ", 7) == 0)
            out->parent = util_strdup(line + 7);
        else if (strncmp(line, "tree ", 5) == 0)
            out->tree = util_strdup(line + 5);
        else if (strncmp(line, "timestamp ", 10) == 0)
            out->timestamp = atol(line + 10);
    }

    fclose(f);
    return 0;
}

int commit_get_parent(const char *id, char *out, size_t size)
{
    kite_commit c;

    if (commit_read(id, &c) != 0)
        return 1;

    if (!c.parent)
        return 1;

    snprintf(out, size, "%s", c.parent);

    return 0;
}

int commit_list(char ***out_ids, size_t *count)
{
    return 1;
}