#ifndef KITE_MERGE_H
#define KITE_MERGE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_merge_result {
    int status;
    char *conflicted_files;
    size_t conflict_count;
} kite_merge_result;

int merge_branches(const char *base_branch, const char *target_branch, kite_merge_result *out);
int merge_commits(const char *base_commit, const char *target_commit, kite_merge_result *out);
void merge_free(kite_merge_result *result);

#ifdef __cplusplus
}
#endif

#endif