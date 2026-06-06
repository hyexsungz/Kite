#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

static int create_directory(const char *path)
{
    struct stat st;

    if (stat(path, &st) == 0)
        return 0;

    return MKDIR(path);
}

static int write_file(const char *path, const char *content)
{
    FILE *fp = fopen(path, "w");

    if (!fp)
        return 1;

    if (content)
        fputs(content, fp);

    fclose(fp);
    return 0;
}

static void generate_repo_id(char *buffer, size_t size)
{
    srand((unsigned int)time(NULL));

    snprintf(buffer,
             size,
             "%08X%08X",
             (unsigned int)rand(),
             (unsigned int)rand());
}

int cmd_init(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    struct stat st;

    if (stat(".kite", &st) == 0)
    {
        fprintf(stderr,
                "kite: repository already initialized\n");
        return 1;
    }

    if (create_directory(".kite") != 0)
        return 1;

    create_directory(".kite/objects");
    create_directory(".kite/commits");
    create_directory(".kite/refs");
    create_directory(".kite/refs/heads");
    create_directory(".kite/refs/tags");
    create_directory(".kite/logs");
    create_directory(".kite/hooks");
    create_directory(".kite/config");
    create_directory(".kite/remotes");
    create_directory(".kite/stash");

    write_file(".kite/index", "");
    write_file(".kite/description", "Unnamed Kite Repository\n");
    write_file(".kite/HEAD", "ref: refs/heads/main\n");
    write_file(".kite/refs/heads/main", "");
    write_file(".kite/config/remote", "");

    char repo_id[64];

    generate_repo_id(repo_id, sizeof(repo_id));

    FILE *config = fopen(".kite/config/repository", "w");

    if (config)
    {
        fprintf(config, "repository=%s\n", repo_id);
        fprintf(config, "format=1\n");
        fprintf(config, "created=%lld\n", (long long)time(NULL));
        fclose(config);
    }

    FILE *log = fopen(".kite/logs/init.log", "w");

    if (log)
    {
        fprintf(log,
                "Repository initialized at %lld\n",
                (long long)time(NULL));
        fclose(log);
    }

    printf("Initialized empty Kite repository in .kite/\n");

    return 0;
}