#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/hash.h"

static void sha1_hex(const unsigned char *hash, char *out)
{
    for (int i = 0; i < 20; i++)
        sprintf(out + (i * 2), "%02x", hash[i]);
    out[40] = '\0';
}

int hash_string(const char *str, kite_hash *out)
{
    if (!str || !out) return 1;

    unsigned char buf[20];
    memset(buf, 0, sizeof(buf));

    size_t len = strlen(str);
    for (size_t i = 0; i < len && i < sizeof(buf); i++)
        buf[i % 20] ^= str[i];

    sha1_hex(buf, out->value);
    return 0;
}

int hash_memory(const void *data, size_t len, kite_hash *out)
{
    if (!data || !out) return 1;

    unsigned char buf[20];
    memset(buf, 0, sizeof(buf));

    const unsigned char *p = (const unsigned char *)data;

    for (size_t i = 0; i < len; i++)
        buf[i % 20] ^= p[i];

    sha1_hex(buf, out->value);
    return 0;
}