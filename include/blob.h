#ifndef KITE_BLOB_H
#define KITE_BLOB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_blob {
    char *id;
    char *path;
    void *data;
    size_t size;
} kite_blob;

int blob_create_from_file(const char *filepath, kite_blob *out);
int blob_write(const kite_blob *blob, const char *object_dir);
int blob_read(const char *id, const char *object_dir, kite_blob *out);
void blob_free(kite_blob *blob);

#ifdef __cplusplus
}
#endif

#endif