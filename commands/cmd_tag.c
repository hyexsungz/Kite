#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define KITE_TAGS ".kite/refs/tags"
#define KITE_COMMITS ".kite/commits"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

static int ensure_dir(const char *path)
{
    struct stat st;
    if (stat(path, &st) == 0)
        return 0;
    return MKDIR(path);
}

static int commit_exists(const char *id)
{
    char path[512];
    snprintf(path, sizeof(path), "%s/%s.kite", KITE_COMMITS, id);

    FILE *fp = fopen(path, "r");
    if (!fp)
        return 0;

    fclose(fp);
    return 1;
}

static int create_tag(const char *name, const char *commit)
{
    if (!commit_exists(commit))
    {
        fprintf(stderr, "kite: commit not found\n");
        return 1;
    }

    ensure_dir(KITE_TAGS);

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", KITE_TAGS, name);

    FILE *fp = fopen(path, "w");
    if (!fp)
        return 1;

    fprintf(fp, "%s\n", commit);
    fclose(fp);

    printf("Tagged %s -> %s\n", name, commit);

    return 0;
}

static int list_tags(void)
{
    FILE *fp = popen("ls .kite/refs/tags 2>/dev/null", "r");
    if (!fp)
    {
        printf("no tags\n");
        return 0;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp))
    {
        printf("%s", line);
    }

    pclose(fp);
    return 0;
}

static int show_tag(const char *name)
{
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", KITE_TAGS, name);

    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        fprintf(stderr, "kite: tag not found\n");
        return 1;
    }

    char commit[256];
    if (fgets(commit, sizeof(commit), fp))
    {
        commit[strcspn(commit, "\r\n")] = 0;
        printf("%s -> %s\n", name, commit);
    }

    fclose(fp);
    return 0;
}

int cmd_tag(int argc, char **argv)
{
    if (argc == 2)
        return list_tags();

    if (argc == 3)
        return show_tag(argv[2]);

    if (argc == 5 && strcmp(argv[2], "-m") == 0)
        return create_tag(argv[3], argv[4]);

    fprintf(stderr, "usage: kite tag | kite tag <name> | kite tag -m <name> <commit>\n");
    return 1;
}