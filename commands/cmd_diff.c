#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KITE_INDEX ".kite/index"
#define MAX_LINE 4096

typedef struct
{
    char hash[128];
    char path[2048];
} IndexEntry;

static int load_index(IndexEntry **entries, size_t *count)
{
    FILE *fp = fopen(KITE_INDEX, "r");

    if (!fp)
        return 1;

    size_t capacity = 32;
    *count = 0;

    *entries = (IndexEntry *)malloc(sizeof(IndexEntry) * capacity);

    if (!*entries)
    {
        fclose(fp);
        return 1;
    }

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp))
    {
        if (*count >= capacity)
        {
            capacity *= 2;

            IndexEntry *tmp =
                (IndexEntry *)realloc(*entries,
                                      sizeof(IndexEntry) * capacity);

            if (!tmp)
            {
                free(*entries);
                fclose(fp);
                return 1;
            }

            *entries = tmp;
        }

        if (sscanf(line,
                   "%127s %2047[^\n]",
                   (*entries)[*count].hash,
                   (*entries)[*count].path) == 2)
        {
            (*count)++;
        }
    }

    fclose(fp);
    return 0;
}

static int compare_files(const char *file1, const char *file2)
{
    FILE *a = fopen(file1, "r");
    FILE *b = fopen(file2, "r");

    if (!a || !b)
    {
        if (a)
            fclose(a);

        if (b)
            fclose(b);

        return 1;
    }

    char line_a[MAX_LINE];
    char line_b[MAX_LINE];

    int line_no = 1;
    int differences = 0;

    while (1)
    {
        char *ra = fgets(line_a, sizeof(line_a), a);
        char *rb = fgets(line_b, sizeof(line_b), b);

        if (!ra && !rb)
            break;

        if (!ra)
            line_a[0] = '\0';

        if (!rb)
            line_b[0] = '\0';

        if (strcmp(line_a, line_b) != 0)
        {
            printf("@@ line %d @@\n", line_no);
            printf("- %s", line_a);
            printf("+ %s", line_b);

            if (line_a[strlen(line_a) - 1] != '\n')
                printf("\n");

            if (line_b[strlen(line_b) - 1] != '\n')
                printf("\n");

            differences++;
        }

        line_no++;
    }

    fclose(a);
    fclose(b);

    return differences;
}

static int diff_working_tree(void)
{
    IndexEntry *entries = NULL;
    size_t count = 0;

    if (load_index(&entries, &count) != 0)
    {
        fprintf(stderr, "kite: unable to read index\n");
        return 1;
    }

    for (size_t i = 0; i < count; i++)
    {
        FILE *fp = fopen(entries[i].path, "r");

        if (!fp)
        {
            printf("deleted: %s\n", entries[i].path);
            continue;
        }

        fclose(fp);
    }

    free(entries);
    return 0;
}

static int diff_two_files(const char *left, const char *right)
{
    printf("--- %s\n", left);
    printf("+++ %s\n", right);

    return compare_files(left, right);
}

int cmd_diff(int argc, char **argv)
{
    if (argc == 2)
    {
        return diff_working_tree();
    }

    if (argc == 4)
    {
        return diff_two_files(argv[2], argv[3]);
    }

    fprintf(stderr,
            "usage: kite diff [file1 file2]\n");

    return 1;
}