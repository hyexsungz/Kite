#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/tree.h"
#include "../include/utils.h"
#include "../include/hash.h"
#include "../include/object.h"

int tree_create(const char *dir_path, char *out_id, size_t size)
{
    if (!dir_path || !out_id)
        return 1;

    kite_hash h;
    hash_string(dir_path, &h);

    snprintf(out_id, size, "%s", h.value);

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", out_id);

    FILE *f = fopen(path, "w");
    if (!f)
        return 1;

    fprintf(f, "tree %s\n", dir_path);
    fclose(f);

    return 0;
}

int tree_read(const char *id, kite_tree *out)
{
    if (!id || !out)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", id);

    FILE *f = fopen(path, "r");
    if (!f)
        return 1;

    memset(out, 0, sizeof(kite_tree));

    out->id = util_strdup(id);

    fclose(f);
    return 0;
}

int tree_write(const kite_tree *tree)
{
    if (!tree || !tree->id)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", tree->id);

    FILE *f = fopen(path, "w");
    if (!f)
        return 1;

    fprintf(f, "tree %s\n", tree->id);
    fclose(f);

    return 0;
}

int tree_list(const char *id, kite_tree_entry **out_entries, size_t *count)
{
    (void)id;
    (void)out_entries;
    (void)count;
    return 1;
}

void tree_free(kite_tree *tree)
{
    if (!tree)
        return;

    free(tree->id);
    free(tree->entries);

    tree->id = NULL;
    tree->entries = NULL;
    tree->entry_count = 0;
}