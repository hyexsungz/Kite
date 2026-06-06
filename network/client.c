#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/network.h"
#include "../include/utils.h"

static int write_file(const char *path, const char *data)
{
    FILE *f = fopen(path, "w");
    if (!f)
        return 1;

    fputs(data, f);
    fclose(f);

    return 0;
}

int net_fetch_remote(const char *remote_url, char **out_data)
{
    if (!remote_url || !out_data)
        return 1;

    FILE *f = fopen(".kite/network.out", "r");
    if (!f)
        return 1;

    char *buf = NULL;
    size_t cap = 0;
    size_t len = 0;

    char line[1024];

    while (fgets(line, sizeof(line), f))
    {
        size_t n = strlen(line);

        char *tmp = realloc(buf, cap + n + 1);
        if (!tmp)
            break;

        buf = tmp;

        memcpy(buf + len, line, n);
        len += n;
        cap += n;
    }

    if (buf)
        buf[len] = 0;

    fclose(f);

    *out_data = buf;
    return 0;
}

int net_fetch_apply(const char *path, const char *data)
{
    if (!path || !data)
        return 1;

    return write_file(path, data);
}

int net_fetch_sync(const char *remote_url, const char *target_path)
{
    if (!remote_url || !target_path)
        return 1;

    char *data = NULL;

    if (net_fetch_remote(remote_url, &data) != 0)
        return 1;

    int res = net_fetch_apply(target_path, data);

    free(data);

    return res;
}