#ifndef KITE_WORKSPACE_H
#define KITE_WORKSPACE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_workspace_file {
    char *path;
    char *hash;
    int status;
} kite_workspace_file;

typedef struct kite_workspace {
    char *root_path;
    kite_workspace_file *files;
    size_t file_count;
} kite_workspace;

int workspace_scan(const char *root, kite_workspace *out);
int workspace_status(const char *root, kite_workspace *out);
int workspace_stage_file(const char *path);
int workspace_unstage_file(const char *path);
int workspace_apply_changes(void);
void workspace_free(kite_workspace *ws);

#ifdef __cplusplus
}
#endif

#endif