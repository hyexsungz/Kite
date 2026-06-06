#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/repository.h"
#include "../include/utils.h"

#define KITE_DIR ".kite"
#define HEAD_FILE ".kite/HEAD"

static int exists(const char *p)
{
    FILE *f = fopen(p, "r");
    if (!f) return 0;
    fclose(f);
    return 1;
}

int repo_init(const char *path)
{
    (void)path;

    system("mkdir .kite");
    system("mkdir .kite\\objects");
    system("mkdir .kite\\refs");
    system("mkdir .kite\\refs\\heads");
    system("mkdir .kite\\refs\\tags");

    FILE *h = fopen(HEAD_FILE, "w");
    if (!h) return 1;

    fputs("ref: refs/heads/main\n", h);
    fclose(h);

    FILE *m = fopen(".kite/refs/heads/main", "w");
    if (m) fclose(m);

    FILE *i = fopen(".kite/index", "w");
    if (i) fclose(i);

    return 0;
}

int repo_open(const char *path, kite_repository *out)
{
    if (!path || !out)
        return 1;

    if (!exists(HEAD_FILE))
        return 1;

    out->path = util_strdup(path);
    out->head = util_strdup(HEAD_FILE);
    out->index_path = util_strdup(".kite/index");
    out->objects_path = util_strdup(".kite/objects");
    out->refs_path = util_strdup(".kite/refs");

    return 0;
}

int repo_close(kite_repository *repo)
{
    if (!repo)
        return 1;

    free(repo->path);
    free(repo->head);
    free(repo->index_path);
    free(repo->objects_path);
    free(repo->refs_path);

    memset(repo, 0, sizeof(*repo));

    return 0;
}

int repo_is_valid(const char *path)
{
    (void)path;
    return exists(HEAD_FILE);
}

int repo_get_head(char *out, size_t size)
{
    FILE *f = fopen(HEAD_FILE, "r");
    if (!f) return 1;

    fgets(out, size, f);
    fclose(f);

    return 0;
}

int repo_set_head(const char *ref)
{
    FILE *f = fopen(HEAD_FILE, "w");
    if (!f) return 1;

    fprintf(f, "ref: %s\n", ref);
    fclose(f);

    return 0;
}