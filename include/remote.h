#ifndef KITE_REMOTE_H
#define KITE_REMOTE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_remote {
    char *name;
    char *url;
    char *branch;
} kite_remote;

int remote_add(const char *name, const char *url);
int remote_remove(const char *name);
int remote_list(char ***out_names, size_t *count);
int remote_get_url(const char *name, char *out, size_t size);
int remote_set_branch(const char *name, const char *branch);

#ifdef __cplusplus
}
#endif

#endif