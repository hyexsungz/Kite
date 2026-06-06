#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../include/blob.h"
#include "../include/utils.h"
#include "../include/hash.h"
#include "../include/object.h"

int blob_create_from_file(const char *filepath, kite_blob *out)
{
    if (!filepath || !out)
        return 1;

    FILE *f = fopen(filepath, "rb");
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

    out->data = data;
    out->size = (size_t)size;
    out->path = util_strdup(filepath);
    out->id = NULL;

    kite_hash h;
    hash_memory(data, (size_t)size, &h);

    out->id = util_strdup(h.value);

    return 0;
}

int blob_write(const kite_blob *blob, const char *object_dir)
{
    if (!blob || !object_dir)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", object_dir, blob->id);

    return util_write_file(path, blob->data, blob->size);
}

int blob_read(const char *id, const char *object_dir, kite_blob *out)
{
    if (!id || !object_dir || !out)
        return 1;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", object_dir, id);

    size_t size;
    char *data;

    if (util_read_file(path, &data, &size) != 0)
        return 1;

    out->data = data;
    out->size = size;
    out->id = util_strdup(id);
    out->path = NULL;

    return 0;
}

void blob_free(kite_blob *blob)
{
    if (!blob)
        return;

    if (blob->data)
        free(blob->data);

    if (blob->id)
        free(blob->id);

    if (blob->path)
        free(blob->path);

    blob->data = NULL;
    blob->id = NULL;
    blob->path = NULL;
    blob->size = 0;
}