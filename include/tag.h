#ifndef KITE_TAG_H
#define KITE_TAG_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_tag {
    char *name;
    char *commit;
    char *message;
} kite_tag;

int tag_create(const char *name, const char *commit, const char *message);
int tag_delete(const char *name);
int tag_list(char ***out_names, size_t *count);
int tag_get(const char *name, kite_tag *out);
int tag_update(const char *name, const char *new_commit);

#ifdef __cplusplus
}
#endif

#endif