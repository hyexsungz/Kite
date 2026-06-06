#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

#define KITE_REMOTE ".kite/config/remote"

static int file_exists(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
        return 0;
    fclose(fp);
    return 1;
}

static int load_remote(char *out, size_t size)
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

static int run_cmd(const char *cmd)
{
    return system(cmd);
}

static int fetch_remote(const char *remote)
{
    char cmd[2048];
    snprintf(cmd, sizeof(cmd), "git fetch \"%s\"", remote);
    return run_cmd(cmd);
}

static int merge_remote_branch(const char *branch)
{
    char cmd[2048];
    snprintf(cmd, sizeof(cmd), "git merge \"%s\"", branch);
    return run_cmd(cmd);
}

static int get_default_branch(char *out, size_t size)
{
    FILE *fp = fopen(".kite/HEAD", "r");
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
    out[size - 1] = 0;

    out[strcspn(out, "\r\n")] = 0;

    return 0;
}

int cmd_pull(int argc, char **argv)
{
    char remote[1024];

    if (argc >= 3)
    {
        strncpy(remote, argv[2], sizeof(remote) - 1);
        remote[sizeof(remote) - 1] = 0;
    }
    else
    {
        if (!file_exists(KITE_REMOTE))
        {
            fprintf(stderr, "kite: no remote configured\n");
            return 1;
        }

        if (load_remote(remote, sizeof(remote)) != 0)
        {
            fprintf(stderr, "kite: failed to load remote\n");
            return 1;
        }
    }

    printf("Pulling from %s\n", remote);

    if (fetch_remote(remote) != 0)
    {
        fprintf(stderr, "kite: fetch failed\n");
        return 1;
    }

    char branch[256];
    if (get_default_branch(branch, sizeof(branch)) == 0)
    {
        if (merge_remote_branch(branch) != 0)
        {
            fprintf(stderr, "kite: merge failed\n");
            return 1;
        }
    }

    printf("Pull complete\n");
    return 0;
}