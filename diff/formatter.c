#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/diff.h"
#include "../include/utils.h"

static const char *type_str(int type)
{
    if (type == 0) return " ";
    if (type == 1) return "+";
    if (type == -1) return "-";
    return "?";
}

int diff_format_unified(const kite_diff *diff, char **out)
{
    if (!diff || !out)
        return 1;

    size_t cap = 1024;
    size_t len = 0;

    char *buf = malloc(cap);
    if (!buf)
        return 1;

    buf[0] = 0;

    for (size_t i = 0; i < diff->line_count; i++)
    {
        const char *prefix = type_str(diff->lines[i].type);
        const char *content = diff->lines[i].content;

        size_t need = strlen(prefix) + strlen(content) + 4;

        if (len + need >= cap)
        {
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if (!tmp)
            {
                free(buf);
                return 1;
            }
            buf = tmp;
        }

        len += snprintf(buf + len, cap - len, "%s %s", prefix, content);
    }

    *out = buf;
    return 0;
}

void diff_print(const kite_diff *diff)
{
    if (!diff)
        return;

    for (size_t i = 0; i < diff->line_count; i++)
    {
        printf("%s %s",
            type_str(diff->lines[i].type),
            diff->lines[i].content);
    }
}

void diff_free(kite_diff *diff)
{
    if (!diff)
        return;

    for (size_t i = 0; i < diff->line_count; i++)
        free(diff->lines[i].content);

    free(diff->lines);
    free(diff->old_path);
    free(diff->new_path);

    diff->lines = NULL;
    diff->line_count = 0;
    diff->old_path = NULL;
    diff->new_path = NULL;
}