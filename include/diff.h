#ifndef KITE_DIFF_H
#define KITE_DIFF_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_diff_line {
    int type;
    char *content;
} kite_diff_line;

typedef struct kite_diff {
    char *old_path;
    char *new_path;
    kite_diff_line *lines;
    size_t line_count;
} kite_diff;

int diff_files(const char *old_file, const char *new_file, kite_diff *out);
int diff_commits(const char *commit_a, const char *commit_b, kite_diff **out, size_t *count);
void diff_free(kite_diff *diff);

#ifdef __cplusplus
}
#endif

#endif