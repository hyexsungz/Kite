#ifndef KITE_COMMIT_H
#define KITE_COMMIT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_commit {
    char *id;
    char *message;
    char *parent;
    char *tree;
    long timestamp;
} kite_commit;

int commit_create(const char *message, char *out_id, size_t size);
int commit_read(const char *id, kite_commit *out);
int commit_write(const kite_commit *commit);
int commit_get_parent(const char *id, char *out, size_t size);
int commit_list(char ***out_ids, size_t *count);

#ifdef __cplusplus
}
#endif

#endif