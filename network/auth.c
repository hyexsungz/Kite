#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/network.h"

static int read_file(const char *path, char **out, size_t *size)
{
    FILE *f = fopen(path, "rb");
    if (!f) return 1;

    fseek(f, 0, SEEK_END);
    long s = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(s + 1);
    if (!buf)
    {
        fclose(f);
        return 1;
    }

    fread(buf, 1, s, f);
    buf[s] = 0;

    fclose(f);

    *out = buf;
    if (size) *size = (size_t)s;

    return 0;
}

int net_client_prepare_payload(const char *repo_path, char **out_payload)
{
    if (!repo_path || !out_payload)
        return 1;

    char *data = NULL;
    size_t size = 0;

    if (read_file(".kite/index", &data, &size) != 0)
        return 1;

    size_t cap = size + 128;
    char *payload = malloc(cap);
    if (!payload)
    {
        free(data);
        return 1;
    }

    snprintf(payload, cap,
        "{ \"repo\": \"%s\", \"index\": \"%s\" }",
        repo_path,
        data ? data : "");

    free(data);

    *out_payload = payload;

    return 0;
}

int net_client_send(const char *url, const char *payload)
{
    if (!url || !payload)
        return 1;

    FILE *f = fopen(".kite/network.out", "a");
    if (!f)
        return 1;

    fprintf(f, "POST %s\n%s\n\n", url, payload);
    fclose(f);

    return 0;
}

int net_client_sync(const char *remote_url)
{
    if (!remote_url)
        return 1;

    char *payload = NULL;

    if (net_client_prepare_payload(".", &payload) != 0)
        return 1;

    int res = net_client_send(remote_url, payload);

    free(payload);

    return res;
}