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

#define KITE_REMOTE_FILE ".kite/config/remote"

static int file_exists(const char *path)
{
    FILE *fp = fopen(path, "r");

    if (!fp)
        return 0;

    fclose(fp);
    return 1;
}

static int load_remote(char *buffer, size_t size)
{
    FILE *fp = fopen(KITE_REMOTE_FILE, "r");

    if (!fp)
        return 1;

    if (!fgets(buffer, (int)size, fp))
    {
        fclose(fp);
        return 1;
    }

    fclose(fp);

    buffer[strcspn(buffer, "\r\n")] = '\0';

    return 0;
}

static int run_fetch(const char *remote)
{
    char command[4096];

    snprintf(command,
             sizeof(command),
             "git fetch \"%s\"",
             remote);

    return system(command);
}

static int print_remote_refs(const char *remote)
{
    char command[4096];

    snprintf(command,
             sizeof(command),
             "git ls-remote \"%s\"",
             remote);

    FILE *pipe = POPEN(command, "r");

    if (!pipe)
        return 1;

    char line[4096];

    while (fgets(line, sizeof(line), pipe))
        fputs(line, stdout);

    PCLOSE(pipe);

    return 0;
}

int cmd_fetch(int argc, char **argv)
{
    char remote[2048];

    if (argc >= 3)
    {
        strncpy(remote,
                argv[2],
                sizeof(remote) - 1);

        remote[sizeof(remote) - 1] = '\0';
    }
    else
    {
        if (!file_exists(KITE_REMOTE_FILE))
        {
            fprintf(stderr,
                    "kite: no remote configured\n");
            return 1;
        }

        if (load_remote(remote,
                        sizeof(remote)) != 0)
        {
            fprintf(stderr,
                    "kite: failed to read remote\n");
            return 1;
        }
    }

    printf("Fetching from %s\n", remote);

    if (run_fetch(remote) != 0)
    {
        fprintf(stderr,
                "kite: fetch failed\n");
        return 1;
    }

    print_remote_refs(remote);

    return 0;
}