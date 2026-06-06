#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/hooks.h"

int post_merge_run(const char *target_branch)
{
    (void)target_branch;

    hook_run(HOOK_POST_MERGE);

    FILE *f = fopen(".kite/HEAD", "r");
    if (!f)
        return 1;

    fclose(f);
    return 0;
}