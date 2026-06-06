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
#define KITE_HEAD ".kite/HEAD"
#define KITE_REFS ".kite/refs/heads"

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

static int get_branch(char *out, size_t size)
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
    out[size - 1] = 0;

    out[strcspn(out, "\r\n")] = 0;

    return 0;
}

static int get_commit(const char *branch, char *out, size_t size)
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

static int run_cmd(const char *cmd)
{
    return system(cmd);
}

static int push_to_remote(const char *remote, const char *branch)
{
    char cmd[4096];
    snprintf(cmd, sizeof(cmd),
             "git push \"%s\" %s",
             remote,
             branch);

    return run_cmd(cmd);
}

int cmd_push(int argc, char **argv)
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

    char branch[256];

    if (get_branch(branch, sizeof(branch)) != 0)
    {
        fprintf(stderr, "kite: cannot determine branch\n");
        return 1;
    }

    char commit[256];

    if (get_commit(branch, commit, sizeof(commit)) != 0)
    {
        fprintf(stderr, "kite: nothing to push\n");
        return 1;
    }

    printf("Pushing %s (%s) to %s\n", branch, commit, remote);

    if (push_to_remote(remote, branch) != 0)
    {
        fprintf(stderr, "kite: push failed\n");
        return 1;
    }

    printf("Push complete\n");

    return 0;
}