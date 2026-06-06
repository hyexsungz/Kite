#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/merge.h"
#include "../include/refs.h"
#include "../include/utils.h"

static int read_ref(const char *path, char *out, size_t size)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return 1;

    fgets(out, size, f);
    fclose(f);

    return 0;
}

int fastforward_check(const char *current_commit, const char *target_commit)
{
    if (!current_commit || !target_commit)
        return 0;

    return strcmp(current_commit, target_commit) != 0;
}

int fastforward_apply(const char *branch_ref, const char *target_commit)
{
    if (!branch_ref || !target_commit)
        return 1;

    FILE *f = fopen(branch_ref, "w");
    if (!f)
        return 1;

    fprintf(f, "%s\n", target_commit);
    fclose(f);

    return 0;
}

int fastforward_merge(const char *current_commit, const char *target_commit, const char *branch_ref)
{
    if (!current_commit || !target_commit || !branch_ref)
        return 1;

    if (!fastforward_check(current_commit, target_commit))
        return 0;

    return fastforward_apply(branch_ref, target_commit);
}

int fastforward_allowed(const char *base, const char *target)
{
    if (!base || !target)
        return 0;

    return strlen(base) < strlen(target);
}