#ifndef KITE_REPOSITORY_H
#define KITE_REPOSITORY_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_repository {
    char *path;
    char *head;
    char *index_path;
    char *objects_path;
    char *refs_path;
} kite_repository;

int repo_init(const char *path);
int repo_open(const char *path, kite_repository *out);
int repo_close(kite_repository *repo);
int repo_is_valid(const char *path);

int repo_get_head(char *out, size_t size);
int repo_set_head(const char *ref);

#ifdef __cplusplus
}
#endif

#endif