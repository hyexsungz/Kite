#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define KITE_INDEX ".kite/index"
#define KITE_HEAD ".kite/HEAD"

static int file_exists(const char *path)
{
    struct stat st;
    return stat(path, &st) == 0;
}

static void get_branch(char *out, size_t size)
{
    FILE *fp = fopen(KITE_HEAD, "r");
    if (!fp)
    {
        strncpy(out, "unknown", size - 1);
        out[size - 1] = 0;
        return;
    }

    char line[512];
    if (!fgets(line, sizeof(line), fp))
    {
        fclose(fp);
        strncpy(out, "unknown", size - 1);
        out[size - 1] = 0;
        return;
    }

    fclose(fp);

    char *ref = strstr(line, "refs/heads/");
    if (!ref)
    {
        strncpy(out, "unknown", size - 1);
        out[size - 1] = 0;
        return;
    }

    ref += strlen("refs/heads/");

    strncpy(out, ref, size - 1);
    out[size - 1] = 0;

    out[strcspn(out, "\r\n")] = 0;
}

static int index_empty(void)
{
    FILE *fp = fopen(KITE_INDEX, "r");
    if (!fp)
        return 1;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);

    fclose(fp);

    return size <= 0;
}

int cmd_status(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    char branch[256];
    get_branch(branch, sizeof(branch));

    printf("On branch %s\n\n", branch);

    if (!file_exists(KITE_INDEX) || index_empty())
    {
        printf("No changes staged for commit\n");
        return 0;
    }

    FILE *fp = fopen(KITE_INDEX, "r");
    if (!fp)
    {
        printf("Nothing to show\n");
        return 0;
    }

    printf("Staged changes:\n");

    char line[4096];
    while (fgets(line, sizeof(line), fp))
    {
        char hash[128], path[2048];

        if (sscanf(line, "%127s %2047[^\n]", hash, path) == 2)
        {
            printf("  modified: %s\n", path);
        }
    }

    fclose(fp);

    return 0;
}