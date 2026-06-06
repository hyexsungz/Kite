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

static int run_command(const char *cmd)
{
    int result = system(cmd);

    if (result != 0)
        return 1;

    return 0;
}

static int is_valid_url(const char *url)
{
    if (!url)
        return 0;

    if (strncmp(url, "http://", 7) == 0)
        return 1;

    if (strncmp(url, "https://", 8) == 0)
        return 1;

    if (strncmp(url, "git://", 6) == 0)
        return 1;

    if (strstr(url, "@"))
        return 1;

    return 0;
}

static void extract_repo_name(const char *url, char *out, size_t out_size)
{
    const char *last = strrchr(url, '/');

    if (!last)
        last = strrchr(url, ':');

    if (!last)
    {
        strncpy(out, "repository", out_size - 1);
        out[out_size - 1] = '\0';
        return;
    }

    last++;

    strncpy(out, last, out_size - 1);
    out[out_size - 1] = '\0';

    char *git = strstr(out, ".git");

    if (git)
        *git = '\0';
}

int cmd_clone(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: kite clone <url> [directory]\n");
        return 1;
    }

    const char *url = argv[2];

    if (!is_valid_url(url))
    {
        fprintf(stderr, "kite: invalid repository URL\n");
        return 1;
    }

    char directory[512];

    if (argc >= 4)
    {
        strncpy(directory, argv[3], sizeof(directory) - 1);
        directory[sizeof(directory) - 1] = '\0';
    }
    else
    {
        extract_repo_name(url, directory, sizeof(directory));
    }

    char command[2048];

    snprintf(command,
             sizeof(command),
             "git clone \"%s\" \"%s\"",
             url,
             directory);

    printf("Cloning into '%s'...\n", directory);

    return run_command(command);
}