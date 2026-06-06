#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/object.h"
#include "../include/utils.h"
#include "../include/hash.h"

int object_write(kite_object_type type, const void *data, size_t size, char *out_id)
{
    if (!data || !out_id)
        return 1;

    kite_hash h;
    hash_memory(data, size, &h);

    snprintf(out_id, 65, "%s", h.value);

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", out_id);

    FILE *f = fopen(path, "wb");
    if (!f)
        return 1;

    fwrite(data, 1, size, f);
    fclose(f);

    return 0;
}

int object_read(const char *id, kite_object *out)
{
    if (!id || !out)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", id);

    FILE *f = fopen(path, "rb");
    if (!f)
        return 1;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    void *data = malloc(size);
    if (!data)
    {
        fclose(f);
        return 1;
    }

    fread(data, 1, size, f);
    fclose(f);

    out->id = util_strdup(id);
    out->data = data;
    out->size = (size_t)size;

    return 0;
}

int object_delete(const char *id)
{
    if (!id)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), ".kite/objects/%s", id);

    return remove(path);
}

int object_exists(const char *id)
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