#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/network.h"
#include "../include/hooks.h"

int net_push_prepare(const char *remote, char **out_payload)
{
    if (!remote || !out_payload)
        return 1;

    FILE *f = fopen(".kite/index", "r");
    if (!f)
        return 1;

    char line[1024];
    size_t cap = 1024;
    size_t len = 0;

    char *buf = malloc(cap);
    if (!buf)
    {
        fclose(f);
        return 1;
    }

    buf[0] = 0;

    while (fgets(line, sizeof(line), f))
    {
        size_t n = strlen(line);

        if (len + n + 64 >= cap)
        {
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if (!tmp)
                break;

            buf = tmp;
        }

        memcpy(buf + len, line, n);
        len += n;
        buf[len] = 0;
    }

    fclose(f);

    char *payload = malloc(len + 128);
    if (!payload)
    {
        free(buf);
        return 1;
    }

    snprintf(payload, len + 128,
        "PUSH:%s\nDATA:%s\n",
        remote,
        buf);

    free(buf);

    *out_payload = payload;

    return 0;
}

int net_push_execute(const char *remote)
{
    if (!remote)
        return 1;

    hook_run(HOOK_PRE_PUSH);

    char *payload = NULL;

    if (net_push_prepare(remote, &payload) != 0)
        return 1;

    FILE *f = fopen(".kite/network.out", "a");
    if (!f)
    {
        free(payload);
        return 1;
    }

    fprintf(f, "%s\n", payload);
    fclose(f);

    free(payload);

    hook_run(HOOK_POST_PUSH);

    return 0;
}