#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KITE_REMOTE ".kite/config/remote"

static int write_remote(const char *url)
{
    FILE *fp = fopen(KITE_REMOTE, "w");
    if (!fp)
        return 1;

    fprintf(fp, "%s\n", url);
    fclose(fp);
    return 0;
}

static int read_remote(char *out, size_t size)
{
    FILE *fp = fopen(KITE_REMOTE, "r");
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

int cmd_remote(int argc, char **argv)
{
    if (argc == 2)
    {
        char remote[1024];

        if (read_remote(remote, sizeof(remote)) != 0)
        {
            printf("kite: no remote configured\n");
            return 0;
        }

        printf("origin %s\n", remote);
        return 0;
    }

    if (argc == 4 && strcmp(argv[2], "add") == 0)
    {
        if (write_remote(argv[3]) != 0)
        {
            fprintf(stderr, "kite: failed to set remote\n");
            return 1;
        }

        printf("remote set\n");
        return 0;
    }

    if (argc == 3 && strcmp(argv[2], "remove") == 0)
    {
        FILE *fp = fopen(KITE_REMOTE, "w");
        if (fp)
            fclose(fp);

        printf("remote removed\n");
        return 0;
    }

    fprintf(stderr, "usage: kite remote | kite remote add <url> | kite remote remove\n");
    return 1;
}