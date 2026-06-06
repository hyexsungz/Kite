#ifndef ERRORS_H
#define ERRORS_H

#include <stddef.h>

void kite_log_error(int level, const char *module, const char *fmt, ...);

int kite_last_error(char *out, size_t size);

void kite_clear_errors(void);

#define KITE_LOG_INFO  0
#define KITE_LOG_WARN  1
#define KITE_LOG_ERROR 2
#define KITE_LOG_FATAL 3

#endif