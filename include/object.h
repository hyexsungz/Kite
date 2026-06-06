#ifndef KITE_OBJECT_H
#define KITE_OBJECT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kite_object_type {
    OBJ_BLOB,
    OBJ_TREE,
    OBJ_COMMIT,
    OBJ_TAG
} kite_object_type;

typedef struct kite_object {
    char *id;
    kite_object_type type;
    void *data;
    size_t size;
} kite_object;

int object_write(kite_object_type type, const void *data, size_t size, char *out_id);
int object_read(const char *id, kite_object *out);
int object_delete(const char *id);
int object_exists(const char *id);

#ifdef __cplusplus
}
#endif

#endif