#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/merge.h"
#include "../include/utils.h"
#include "../include/hooks.h"

int merge_engine_three_way(const char *base, const char *a, const char *b, kite_merge_result *out)
{
    if (!base || !a || !b || !out)
        return 1;

    FILE *fb = fopen(base, "r");
    FILE *fa = fopen(a, "r");
    FILE *fc = fopen(b, "r");

    if (!fb || !fa || !fc)
        return 1;

    char lb[1024], la[1024], lc[1024];

    out->status = 0;
    out->conflict_count = 0;
    out->conflicted_files = NULL;

    while (1)
    {
        char *rb = fgets(lb, sizeof(lb), fb);
        char *ra = fgets(la, sizeof(la), fa);
        char *rc = fgets(lc, sizeof(lc), fc);

        if (!rb && !ra && !rc)
            break;

        if (ra && rc && strcmp(la, lc) != 0)
        {
            out->status = 1;
            out->conflict_count++;

            size_t need = strlen(la) + strlen(lc) + 32;

            char *tmp = realloc(out->conflicted_files,
                                (out->conflict_count) * need);

            if (tmp)
            {
                out->conflicted_files = tmp;

                strcat(out->conflicted_files, "<<<<<<< A\n");
                strcat(out->conflicted_files, la);
                strcat(out->conflicted_files, "=======\n");
                strcat(out->conflicted_files, lc);
                strcat(out->conflicted_files, ">>>>>>> B\n");
            }
        }
    }

    fclose(fb);
    fclose(fa);
    fclose(fc);

    return out->status;
}

int merge_engine_simple(const char *a, const char *b, kite_merge_result *out)
{
    if (!a || !b || !out)
        return 1;

    FILE *fa = fopen(a, "r");
    FILE *fb = fopen(b, "r");

    if (!fa || !fb)
        return 1;

    out->status = 0;
    out->conflicted_files = NULL;
    out->conflict_count = 0;

    char la[1024], lb[1024];

    while (fgets(la, sizeof(la), fa) && fgets(lb, sizeof(lb), fb))
    {
        if (strcmp(la, lb) != 0)
        {
            out->status = 1;
            out->conflict_count++;
        }
    }

    fclose(fa);
    fclose(fb);

    return out->status;
}

int merge_engine_finalize(kite_merge_result *res)
{
    if (!res)
        return 1;

    hook_run(HOOK_POST_MERGE);
    return 0;
}