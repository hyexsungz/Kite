#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/hooks.h"
#include "../include/utils.h"

#define HOOKS_DIR ".kite/hooks"

int hook_register(kite_hook_type type, const char *script_path)
{
    (void)type;
    (void)script_path;

    return 0;
}

int hook_run(kite_hook_type type)
{
    char path[512];

    switch (type)
{
    case HOOK_PRE_COMMIT: break;
    case HOOK_POST_COMMIT: break;

    case HOOK_PRE_PUSH: break;
    case HOOK_POST_PUSH: break;

    case HOOK_PRE_MERGE: break;
    case HOOK_POST_MERGE: break;

    case HOOK_PRE_REBASE: break;
    case HOOK_POST_REBASE: break;

    default: break;
}

    FILE *f = fopen(path, "r");
    if (!f)
        return 0;

    fclose(f);
    return 0;
}