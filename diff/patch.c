#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/diff.h"
#include "../include/utils.h"

static int apply_line(FILE *out, const char *line)
{
    if (!line)
        return 1;

    fputs(line, out);
    return 0;
}

int patch_apply(const char *file_path, const kite_diff *diff)
{
    if (!file_path || !diff)
        return 1;

    FILE *f = fopen(file_path, "r");
    if (!f)
        return 1;

    char **lines = NULL;
    size_t count = 0;

    char buf[1024];

    while (fgets(buf, sizeof(buf), f))
    {
        char **tmp = realloc(lines, sizeof(char *) * (count + 1));
        if (!tmp)
            break;

        lines = tmp;
        lines[count] = util_strdup(buf);
        count++;
    }

    fclose(f);

    FILE *out = fopen(file_path, "w");
    if (!out)
        return 1;

    size_t d = 0;

    for (size_t i = 0; i < diff->line_count; i++)
    {
        if (diff->lines[i].type == -1)
        {
            continue;
        }
        else if (diff->lines[i].type == 1)
        {
            apply_line(out, diff->lines[i].content);
        }
        else
        {
            if (d < count)
                apply_line(out, lines[d++]);
        }
    }

    for (size_t i = 0; i < count; i++)
        free(lines[i]);

    free(lines);
    fclose(out);

    return 0;
}

int patch_create(const kite_diff *diff, char **out_patch)
{
    if (!diff || !out_patch)
        return 1;

    size_t cap = 1024;
    size_t len = 0;

    char *buf = malloc(cap);
    if (!buf)
        return 1;

    buf[0] = 0;

    for (size_t i = 0; i < diff->line_count; i++)
    {
        const char *prefix = "";

        if (diff->lines[i].type == 1)
            prefix = "+";
        else if (diff->lines[i].type == -1)
            prefix = "-";
        else
            prefix = " ";

        size_t need = strlen(prefix) + strlen(diff->lines[i].content) + 2;

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

        len += snprintf(buf + len, cap - len, "%s%s", prefix, diff->lines[i].content);
    }

    *out_patch = buf;
    return 0;
}