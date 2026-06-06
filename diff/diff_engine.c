#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/diff.h"
#include "../include/utils.h"

static int line_eq(const char *a, const char *b)
{
    if (!a || !b)
        return 0;
    return strcmp(a, b) == 0;
}

static void push_line(kite_diff *out, int type, const char *content)
{
    kite_diff_line *tmp = realloc(out->lines, sizeof(kite_diff_line) * (out->line_count + 1));
    if (!tmp)
        return;

    out->lines = tmp;
    out->lines[out->line_count].type = type;
    out->lines[out->line_count].content = util_strdup(content);
    out->line_count++;
}

int diff_files(const char *old_file, const char *new_file, kite_diff *out)
{
    if (!old_file || !new_file || !out)
        return 1;

    FILE *a = fopen(old_file, "r");
    FILE *b = fopen(new_file, "r");

    if (!a || !b)
        return 1;

    char la[1024], lb[1024];

    out->lines = NULL;
    out->line_count = 0;
    out->old_path = util_strdup(old_file);
    out->new_path = util_strdup(new_file);

    while (1)
    {
        char *ra = fgets(la, sizeof(la), a);
        char *rb = fgets(lb, sizeof(lb), b);

        if (!ra && !rb)
            break;

        if (ra && rb)
        {
            if (line_eq(la, lb))
            {
                push_line(out, 0, la);
            }
            else
            {
                push_line(out, -1, la);
                push_line(out, 1, lb);
            }
        }
        else if (ra && !rb)
        {
            push_line(out, -1, la);
        }
        else if (!ra && rb)
        {
            push_line(out, 1, lb);
        }
    }

    fclose(a);
    fclose(b);

    return 0;
}

int diff_commits(const char *commit_a, const char *commit_b, kite_diff **out, size_t *count)
{
    (void)commit_a;
    (void)commit_b;

    if (!out || !count)
        return 1;

    *out = NULL;
    *count = 0;

    return 0;
}