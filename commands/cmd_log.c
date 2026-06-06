#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KITE_HEAD ".kite/HEAD"
#define KITE_REFS ".kite/refs/heads"
#define KITE_COMMITS ".kite/commits"

static int get_current_branch(char *out, size_t size)
{
    FILE *fp = fopen(KITE_HEAD, "r");
    if (!fp)
        return 1;

    char line[512];
    if (!fgets(line, sizeof(line), fp))
    {
        fclose(fp);
        return 1;
    }

    fclose(fp);

    char *ref = strstr(line, "refs/heads/");
    if (!ref)
        return 1;

    ref += strlen("refs/heads/");

    strncpy(out, ref, size - 1);
    out[size - 1] = '\0';

    out[strcspn(out, "\r\n")] = 0;

    return 0;
}

static int get_head_commit(const char *branch, char *out, size_t size)
{
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", KITE_REFS, branch);

    FILE *fp = fopen(path, "r");
    if (!fp)
        return 1;

    if (!fgets(out, (int)size, fp))
    {
        fclose(fp);
        return 1;
    }

    fclose(fp);

    out[strcspn(out, "\r\n")] = 0;

    return 0;
}

static int print_commit(const char *commit_id)
{
    char path[512];
    snprintf(path, sizeof(path), "%s/%s.kite", KITE_COMMITS, commit_id);

    FILE *fp = fopen(path, "r");
    if (!fp)
        return 1;

    char line[1024];

    printf("commit %s\n", commit_id);

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "commit ", 7) == 0)
            continue;

        printf("%s", line);
    }

    fclose(fp);
    printf("\n");

    return 0;
}

static int walk_history(const char *start_commit)
{
    char current[256];
    strncpy(current, start_commit, sizeof(current) - 1);
    current[sizeof(current) - 1] = 0;

    while (strlen(current) > 0)
    {
        print_commit(current);

        char path[512];
        snprintf(path, sizeof(path), "%s/%s.kite", KITE_COMMITS, current);

        FILE *fp = fopen(path, "r");
        if (!fp)
            break;

        char line[1024];
        char parent[256] = {0};

        while (fgets(line, sizeof(line), fp))
        {
            if (strncmp(line, "parent ", 7) == 0)
            {
                sscanf(line, "parent %255s", parent);
                break;
            }
        }

        fclose(fp);

        if (strlen(parent) == 0)
            break;

        strncpy(current, parent, sizeof(current) - 1);
        current[sizeof(current) - 1] = 0;
    }

    return 0;
}

int cmd_log(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    char branch[256];

    if (get_current_branch(branch, sizeof(branch)) != 0)
    {
        fprintf(stderr, "kite: cannot determine branch\n");
        return 1;
    }

    char commit[256];

    if (get_head_commit(branch, commit, sizeof(commit)) != 0)
    {
        fprintf(stderr, "kite: no commits found\n");
        return 1;
    }

    return walk_history(commit);
}