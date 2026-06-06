#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/hooks.h"
#include "../include/utils.h"

int post_commit_run(const char *commit_id)
{
    (void)commit_id;

    hook_run(HOOK_POST_COMMIT);

    FILE *f = fopen(".kite/HEAD", "r");
    if (!f)
        return 1;

    fclose(f);
    return 0;
}