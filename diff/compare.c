#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/diff.h"
#include "../include/utils.h"

char **compare_read_lines(const char *path, size_t *count)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return NULL;

    char **lines = NULL;
    char buf[1024];
    size_t n = 0;

    while (fgets(buf, sizeof(buf), f))
    {
        char **tmp = realloc(lines, sizeof(char *) * (n + 1));
        if (!tmp)
        {
            fclose(f);
            return NULL;
        }

        lines = tmp;
        lines[n] = util_strdup(buf);

        if (!lines[n])
        {
            fclose(f);
            return NULL;
        }

        n++;
    }

    fclose(f);
    *count = n;
    return lines;
}

int compare_files(const char *a_path, const char *b_path)
{
    if (!a_path || !b_path)
        return 1;

    size_t a_count = 0, b_count = 0;

    char **a = compare_read_lines(a_path, &a_count);
    char **b = compare_read_lines(b_path, &b_count);

    if (!a || !b)
        return 1;

    size_t max = (a_count > b_count) ? a_count : b_count;

    for (size_t i = 0; i < max; i++)
    {
        const char *line_a = (i < a_count) ? a[i] : "";
        const char *line_b = (i < b_count) ? b[i] : "";

        if (strcmp(line_a, line_b) != 0)
        {
            printf("- %s+ %s", line_a, line_b);
        }
    }

    for (size_t i = 0; i < a_count; i++)
        free(a[i]);

    for (size_t i = 0; i < b_count; i++)
        free(b[i]);

    free(a);
    free(b);

    return 0;
}