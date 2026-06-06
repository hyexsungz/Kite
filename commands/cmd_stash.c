#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sys/stat.h"

#define KITE_STASH_DIR ".kite/stash"
#define KITE_INDEX ".kite/index"
#define KITE_HEAD ".kite/HEAD"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

static int ensure_dir(const char *path)
{
    struct stat st;
    if (stat(path, &st) == 0)
        return 0;
    return MKDIR(path);
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

static int stash_save(const char *message)
{
    ensure_dir(KITE_STASH_DIR);

    char branch[256];
    if (get_branch(branch, sizeof(branch)) != 0)
        strcpy(branch, "unknown");

    char stash_id[64];
    snprintf(stash_id, sizeof(stash_id), "stash_%ld", (long)time(NULL));

    char path[512];
    snprintf(path, sizeof(path), "%s/%s.kite", KITE_STASH_DIR, stash_id);

    FILE *stash = fopen(path, "w");
    if (!stash)
        return 1;

    fprintf(stash, "stash %s\n", stash_id);
    fprintf(stash, "branch %s\n", branch);
    fprintf(stash, "message %s\n\n", message ? message : "stash");

    FILE *index = fopen(KITE_INDEX, "r");
    if (index)
    {
        char line[4096];
        while (fgets(line, sizeof(line), index))
            fputs(line, stash);

        fclose(index);
    }

    fclose(stash);

    FILE *clear = fopen(KITE_INDEX, "w");
    if (clear)
        fclose(clear);

    printf("Saved working directory as %s\n", stash_id);

    return 0;
}

static int stash_list(void)
{
    FILE *fp = popen("ls .kite/stash 2>/dev/null", "r");
    if (!fp)
    {
        printf("no stashes\n");
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

static int stash_apply(const char *id)
{
    char path[512];
    snprintf(path, sizeof(path), "%s/%s.kite", KITE_STASH_DIR, id);

    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        fprintf(stderr, "kite: stash not found\n");
        return 1;
    }

    printf("Applying stash %s\n", id);

    char line[4096];
    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, "stash ", 6) == 0)
            continue;
        if (strncmp(line, "branch ", 7) == 0)
            continue;
        if (strncmp(line, "message ", 8) == 0)
            continue;
    }

    fclose(fp);

    return 0;
}

int cmd_stash(int argc, char **argv)
{
    if (argc == 2)
        return stash_list();

    if (argc == 3 && strcmp(argv[2], "save") == 0)
        return stash_save("stash");

    if (argc == 4 && strcmp(argv[2], "save") == 0)
        return stash_save(argv[3]);

    if (argc == 4 && strcmp(argv[2], "apply") == 0)
        return stash_apply(argv[3]);

    fprintf(stderr, "usage: kite stash | stash save [msg] | stash apply <id>\n");
    return 1;
}