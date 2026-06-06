#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/utils.h"
#include "../include/object.h"
#include "../include/hash.h"

int storage_write_blob(const void *data, size_t size, char *out_id)
{
    if (!data || !out_id)
        return 1;

    kite_hash h;
    hash_memory(data, size, &h);

    snprintf(out_id, 65, "%s", h.value);

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", out_id);

    return util_write_file(path, data, size);
}

int storage_read_blob(const char *id, void **out_data, size_t *out_size)
{
    if (!id || !out_data || !out_size)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", id);

    return util_read_file(path, (char **)out_data, out_size);
}

int storage_delete_object(const char *id)
{
    if (!id)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", id);

    return remove(path);
}

int storage_exists(const char *id)
{
    if (!id)
        return 0;

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", id);

    FILE *f = fopen(path, "rb");
    if (!f)
        return 0;

    fclose(f);
    return 1;
}