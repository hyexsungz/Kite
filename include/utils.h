#ifndef KITE_UTILS_H
#define KITE_UTILS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int util_read_file(const char *path, char **out_data, size_t *out_size);
int util_write_file(const char *path, const void *data, size_t size);
int util_file_exists(const char *path);
int util_make_dir(const char *path);
int kite_mkdir(const char *path);

char *util_strdup(const char *str);
char *util_join_path(const char *a, const char *b);

long util_timestamp(void);

#ifdef __cplusplus
}
#endif

#endif