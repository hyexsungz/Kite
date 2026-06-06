#ifndef KITE_REFS_H
#define KITE_REFS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_ref {
    char *name;
    char *target;
} kite_ref;

int ref_init(const char *repo_path);
int ref_create(const char *name, const char *target);
int ref_delete(const char *name);
int ref_update(const char *name, const char *new_target);
int ref_resolve(const char *name, char *out, size_t size);
int ref_list(char ***out_names, size_t *count);

#ifdef __cplusplus
}
#endif

#endif