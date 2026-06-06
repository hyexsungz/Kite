#ifndef KITE_TREE_H
#define KITE_TREE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_tree_entry {
    char *name;
    char *hash;
    int mode;
} kite_tree_entry;

typedef struct kite_tree {
    char *id;
    kite_tree_entry *entries;
    size_t entry_count;
} kite_tree;

int tree_create(const char *dir_path, char *out_id, size_t size);
int tree_read(const char *id, kite_tree *out);
int tree_write(const kite_tree *tree);
int tree_list(const char *id, kite_tree_entry **out_entries, size_t *count);
void tree_free(kite_tree *tree);

#ifdef __cplusplus
}
#endif

#endif