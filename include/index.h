#ifndef KITE_INDEX_H
#define KITE_INDEX_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_index_entry {
    char *path;
    char *hash;
    int stage;
} kite_index_entry;

int index_init(const char *path);
int index_add(const char *file);
int index_remove(const char *file);
int index_clear(void);
int index_write(void);
int index_read(void);
int index_list(char ***out_paths, size_t *count);
int index_has_changes(void);

#ifdef __cplusplus
}
#endif

#endif