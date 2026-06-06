#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#endif

#include "../include/refs.h"
#include "../include/utils.h"

#define REFS_DIR ".kite/refs"
#define HEAD_FILE ".kite/HEAD"

int ensure_dir(const char *path)
{
    return kite_mkdir(path);
}

int ref_init(const char *repo_path)
{
    (void)repo_path;

    ensure_dir(".kite");
    ensure_dir(REFS_DIR);
    ensure_dir(".kite/refs/heads");
    ensure_dir(".kite/refs/tags");

    FILE *f = fopen(HEAD_FILE, "w");
    if (!f)
        return 1;

    fputs("ref: refs/heads/main\n", f);
    fclose(f);

    FILE *m = fopen(".kite/refs/heads/main", "w");
    if (m)
        fclose(m);

    return 0;
}

int ref_create(const char *name, const char *target)
{
    if (!name || !target)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), ".kite/refs/%s", name);

    FILE *f = fopen(path, "w");
    if (!f)
        return 1;

    fprintf(f, "%s", target);
    fclose(f);

    return 0;
}

int ref_delete(const char *name)
{
    if (!name)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), ".kite/refs/%s", name);

    return remove(path);
}

int ref_update(const char *name, const char *new_target)
{
    return ref_create(name, new_target);
}

int ref_resolve(const char *name, char *out, size_t size)
{
    if (!name || !out)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), ".kite/refs/%s", name);

    FILE *f = fopen(path, "r");
    if (!f)
        return 1;

    fgets(out, size, f);
    fclose(f);

    return 0;
}

int ref_list(char ***out_names, size_t *count)
{
    (void)out_names;
    (void)count;
    return 1;
}