#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/index.h"
#include "../include/utils.h"
#include "../include/hash.h"

#define INDEX_FILE ".kite/index"

typedef struct {
    char *path;
    char *hash;
} entry_t;

static entry_t *entries = NULL;
static size_t entry_count = 0;

static void free_entries(void)
{
    for (size_t i = 0; i < entry_count; i++)
    {
        free(entries[i].path);
        free(entries[i].hash);
    }
    free(entries);
    entries = NULL;
    entry_count = 0;
}

int index_init(const char *path)
{
    (void)path;
    FILE *f = fopen(INDEX_FILE, "w");
    if (!f)
        return 1;
    fclose(f);
    return 0;
}

int index_add(const char *file)
{
    if (!file)
        return 1;

    FILE *f = fopen(INDEX_FILE, "a");
    if (!f)
        return 1;

    kite_hash h;
    hash_string(file, &h);

    fprintf(f, "%s %s\n", file, h.value);
    fclose(f);

    return 0;
}

int index_remove(const char *file)
{
    if (!file)
        return 1;

    FILE *f = fopen(INDEX_FILE, "r");
    if (!f)
        return 1;

    FILE *tmp = fopen(".kite/index.tmp", "w");
    if (!tmp)
    {
        fclose(f);
        return 1;
    }

    char p[512], h[128];

    while (fscanf(f, "%s %s", p, h) == 2)
    {
        if (strcmp(p, file) != 0)
            fprintf(tmp, "%s %s\n", p, h);
    }

    fclose(f);
    fclose(tmp);

    remove(INDEX_FILE);
    rename(".kite/index.tmp", INDEX_FILE);

    return 0;
}

int index_clear(void)
{
    FILE *f = fopen(INDEX_FILE, "w");
    if (!f)
        return 1;
    fclose(f);
    return 0;
}

int index_write(void)
{
    return 0;
}

int index_read(void)
{
    return 0;
}

int index_list(char ***out_paths, size_t *count)
{
    return 1;
}

int index_has_changes(void)
{
    FILE *f = fopen(INDEX_FILE, "r");
    if (!f)
        return 0;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);

    return size > 0;
}