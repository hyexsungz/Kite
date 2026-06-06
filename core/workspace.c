#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../include/workspace.h"
#include "../include/utils.h"
#include "../include/hash.h"
#include "../include/index.h"

static int is_file(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    fclose(f);
    return 1;
}

static char *read_file(const char *path, size_t *size)
{
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long s = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(s);
    if (!buf)
    {
        fclose(f);
        return NULL;
    }

    fread(buf, 1, s, f);
    fclose(f);

    if (size) *size = (size_t)s;
    return buf;
}

int workspace_scan(const char *root, kite_workspace *out)
{
    (void)root;
    if (!out)
        return 1;

    out->root_path = util_strdup(".");
    out->files = NULL;
    out->file_count = 0;

    return 0;
}

int workspace_status(const char *root, kite_workspace *out)
{
    (void)root;

    FILE *f = fopen(".kite/index", "r");
    if (!f)
        return 1;

    out->root_path = util_strdup(".");

    char path[512];
    char hash[128];

    while (fscanf(f, "%s %s", path, hash) == 2)
    {
        kite_workspace_file *tmp =
            realloc(out->files, sizeof(kite_workspace_file) * (out->file_count + 1));

        if (!tmp)
            break;

        out->files = tmp;

        out->files[out->file_count].path = util_strdup(path);
        out->files[out->file_count].hash = util_strdup(hash);
        out->files[out->file_count].status = 0;

        out->file_count++;
    }

    fclose(f);
    return 0;
}

int workspace_stage_file(const char *path)
{
    return index_add(path);
}

int workspace_unstage_file(const char *path)
{
    return index_remove(path);
}

int workspace_apply_changes(void)
{
    return 0;
}

void workspace_free(kite_workspace *ws)
{
    if (!ws)
        return;

    for (size_t i = 0; i < ws->file_count; i++)
    {
        free(ws->files[i].path);
        free(ws->files[i].hash);
    }

    free(ws->files);
    free(ws->root_path);

    ws->files = NULL;
    ws->root_path = NULL;
    ws->file_count = 0;
}