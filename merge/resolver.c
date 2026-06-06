#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/merge.h"
#include "../include/hooks.h"

int merge_branches(const char *base_branch, const char *target_branch, kite_merge_result *out)
{
    if (!base_branch || !target_branch || !out)
        return 1;

    hook_run(HOOK_PRE_MERGE);

    out->status = 0;
    out->conflict_count = 0;
    out->conflicted_files = NULL;

    FILE *a = fopen(base_branch, "r");
    FILE *b = fopen(target_branch, "r");

    if (!a || !b)
        return 1;

    char la[1024], lb[1024];

    while (fgets(la, sizeof(la), a) && fgets(lb, sizeof(lb), b))
    {
        if (strcmp(la, lb) != 0)
        {
            out->status = 1;
            out->conflict_count++;

            size_t need = strlen(la) + strlen(lb) + 64;

            char *tmp = realloc(out->conflicted_files,
                                (out->conflict_count) * need);

            if (tmp)
            {
                out->conflicted_files = tmp;

                strcat(out->conflicted_files, "<<<<<<< BASE\n");
                strcat(out->conflicted_files, la);
                strcat(out->conflicted_files, "=======\n");
                strcat(out->conflicted_files, lb);
                strcat(out->conflicted_files, ">>>>>>> TARGET\n");
            }
        }
    }

    fclose(a);
    fclose(b);

    hook_run(HOOK_POST_MERGE);

    return out->status;
}

int merge_commits(const char *base_commit, const char *target_commit, kite_merge_result *out)
{
    if (!base_commit || !target_commit || !out)
        return 1;

    FILE *a = fopen(base_commit, "r");
    FILE *b = fopen(target_commit, "r");

    if (!a || !b)
        return 1;

    out->status = 0;
    out->conflict_count = 0;
    out->conflicted_files = NULL;

    char la[1024], lb[1024];

    while (fgets(la, sizeof(la), a) && fgets(lb, sizeof(lb), b))
    {
        if (strcmp(la, lb) != 0)
        {
            out->status = 1;
            out->conflict_count++;
        }
    }

    fclose(a);
    fclose(b);

    return out->status;
}

void merge_free(kite_merge_result *result)
{
    if (!result)
        return;

    free(result->conflicted_files);

    result->conflicted_files = NULL;
    result->conflict_count = 0;
    result->status = 0;
}