#ifndef HASH_H
#define HASH_H

#include <stddef.h>

typedef struct {
    char value[65];
} kite_hash;

int hash_memory(const void *data, size_t len, kite_hash *out);
int hash_string(const char *str, kite_hash *out);

#endif