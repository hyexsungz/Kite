#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/hooks.h"
#include "../include/remote.h"

int pre_push_run(const char *remote_name)
{
    if (!remote_name)
        return 1;

    hook_run(HOOK_PRE_PUSH);

    char url[512];
    if (remote_get_url(remote_name, url, sizeof(url)) != 0)
        return 1;

    FILE *f = fopen(".kite/index", "r");
    if (!f)
        return 1;

    fclose(f);

    return 0;
}