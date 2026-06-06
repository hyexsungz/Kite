#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/hooks.h"
#include "../include/index.h"
#include "../include/utils.h"

int pre_commit_run(void)
{
    if (!index_has_changes())
        return 1;

    hook_run(HOOK_PRE_COMMIT);

    FILE *f = fopen(".kite/index", "r");
    if (!f)
        return 1;

    char line[512];

    while (fgets(line, sizeof(line), f))
    {
        if (strstr(line, ".") == NULL)
            return 1;
    }

    fclose(f);
    return 0;
}