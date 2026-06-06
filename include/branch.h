#ifndef KITE_BRANCH_H
#define KITE_BRANCH_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_branch {
    char *name;
    char *commit_id;
} kite_branch;

int branch_create(const char *name);
int branch_delete(const char *name);
int branch_list(char ***out_names, size_t *count);
int branch_get_current(char *out, size_t size);
int branch_set_current(const char *name);
int branch_resolve(const char *name, char *out_commit, size_t size);

#ifdef __cplusplus
}
#endif

#endif