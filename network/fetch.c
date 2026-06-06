#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/network.h"

int protocol_encode_push(const char *ref, const char *data, char **out)
{
    if (!ref || !data || !out)
        return 1;

    size_t len = strlen(ref) + strlen(data) + 64;

    char *buf = malloc(len);
    if (!buf)
        return 1;

    snprintf(buf, len,
        "PUSH\nREF:%s\nDATA:%s\nEND\n",
        ref, data);

    *out = buf;
    return 0;
}

int protocol_encode_fetch(const char *ref, char **out)
{
    if (!ref || !out)
        return 1;

    size_t len = strlen(ref) + 32;

    char *buf = malloc(len);
    if (!buf)
        return 1;

    snprintf(buf, len,
        "FETCH\nREF:%s\nEND\n",
        ref);

    *out = buf;
    return 0;
}

int protocol_decode(const char *packet, char **type, char **data)
{
    if (!packet || !type || !data)
        return 1;

    if (strncmp(packet, "PUSH", 4) == 0)
        *type = "PUSH";
    else if (strncmp(packet, "FETCH", 5) == 0)
        *type = "FETCH";
    else
        *type = "UNKNOWN";

    const char *p = strstr(packet, "DATA:");
    if (p)
        *data = (char *)(p + 5);
    else
        *data = NULL;

    return 0;
}

int protocol_validate(const char *packet)
{
    if (!packet)
        return 1;

    if (strstr(packet, "END\n") == NULL)
        return 1;

    return 0;
}