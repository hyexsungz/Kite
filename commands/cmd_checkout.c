#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KITE_HEAD ".kite/HEAD"
#define KITE_REFS ".kite/refs/heads"

static int branch_exists(const char *name)
{
    char path[1024];

    snprintf(path,
             sizeof(path),
             "%s/%s",
             KITE_REFS,
             name);

    FILE *fp = fopen(path, "r");

    if (!fp)
        return 0;

    fclose(fp);
    return 1;
}

static int read_branch_commit(const char *name, char *buffer, size_t size)
{
    char path[1024];

    snprintf(path,
             sizeof(path),
             "%s/%s",
             KITE_REFS,
             name);

    FILE *fp = fopen(path, "r");

    if (!fp)
        return 1;

    if (!fgets(buffer, (int)size, fp))
        buffer[0] = '\0';

    fclose(fp);

    buffer[strcspn(buffer, "\r\n")] = '\0';

    return 0;
}

static int write_head(const char *branch)
{
    FILE *fp = fopen(KITE_HEAD, "w");

    if (!fp)
        return 1;

    fprintf(fp, "ref: refs/heads/%s\n", branch);

    fclose(fp);

    return 0;
}

static int checkout_branch(const char *branch)
{
    if (!branch_exists(branch))
    {
        fprintf(stderr, "kite: branch '%s' does not exist\n", branch);
        return 1;
    }

    if (write_head(branch) != 0)
    {
        fprintf(stderr, "kite: failed to update HEAD\n");
        return 1;
    }

    char commit_id[256];

    if (read_branch_commit(branch,
                           commit_id,
                           sizeof(commit_id)) == 0)
    {
        printf("Switched to branch '%s'\n", branch);
    }

    return 0;
}

int cmd_checkout(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: kite checkout <branch>\n");
        return 1;
    }

    return checkout_branch(argv[2]);
}