#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/utils.h"

static const char *level_str(int level)
{
    if (level == 0) return "INFO";
    if (level == 1) return "WARN";
    if (level == 2) return "ERROR";
    return "DEBUG";
}

static void get_time(char *out, size_t size)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    strftime(out, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

void log_write(int level, const char *module, const char *msg)
{
    FILE *f = fopen(".kite/runtime.log", "a");
    if (!f)
        return;

    char timebuf[64];
    get_time(timebuf, sizeof(timebuf));

    fprintf(f, "[%s] [%s] (%s): %s\n",
            timebuf,
            level_str(level),
            module ? module : "core",
            msg ? msg : "");

    fclose(f);
}

void log_info(const char *module, const char *msg)
{
    log_write(0, module, msg);
}

void log_warn(const char *module, const char *msg)
{
    log_write(1, module, msg);
}

void log_error(const char *module, const char *msg)
{
    log_write(2, module, msg);
}