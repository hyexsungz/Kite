#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../include/errors.h"
#include "../include/utils.h"

static const char *level_str(int level)
{
    if (level == 0) return "INFO";
    if (level == 1) return "WARN";
    if (level == 2) return "ERROR";
    if (level == 3) return "FATAL";
    return "UNKNOWN";
}

void kite_error(int level, const char *module, const char *fmt, ...)
{
    FILE *f = fopen(".kite/error.log", "a");
    if (!f)
        return;

    fprintf(f, "[%s] (%s): ", level_str(level), module ? module : "core");

    va_list args;
    va_start(args, fmt);
    vfprintf(f, fmt, args);
    va_end(args);

    fprintf(f, "\n");

    fclose(f);
}

int kite_last_error(char *out, size_t size)
{
    if (!out)
        return 1;

    FILE *f = fopen(".kite/error.log", "r");
    if (!f)
        return 1;

    char line[1024];
    char last[1024] = {0};

    while (fgets(line, sizeof(line), f))
    {
        strncpy(last, line, sizeof(last) - 1);
    }

    fclose(f);

    snprintf(out, size, "%s", last);
    return 0;
}

void kite_clear_errors(void)
{
    FILE *f = fopen(".kite/error.log", "w");
    if (f)
        fclose(f);
}