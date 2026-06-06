#ifndef KITE_LOGGER_H
#define KITE_LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kite_log_level {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
} kite_log_level;

void log_set_level(kite_log_level level);
void log_debug(const char *msg);
void log_info(const char *msg);
void log_warn(const char *msg);
void log_error(const char *msg);
void log_fatal(const char *msg);

void log_format(kite_log_level level, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif