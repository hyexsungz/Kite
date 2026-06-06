#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "../include/config.h"

static int exists(const char *p)
{
    struct stat st;
    return stat(p, &st) == 0;
}

static int mk(const char *p)
{
#ifdef _WIN32
    return _mkdir(p);
#else
    return mkdir(p, 0755);
#endif
}

int sys_repo_init(void)
{
    if (exists(KITE_DIR))
    {
        printf("kite: already initialized\n");
        return 1;
    }

    mk(KITE_DIR);
    mk(KITE_COMMITS);
    mk(KITE_REFS);
    mk(KITE_REFS "/heads");
    mk(KITE_REFS "/tags");
    mk(".kite/config");
    mk(".kite/stash");
    mk(".kite/objects");

    FILE *f;

    f = fopen(KITE_INDEX, "w"); if (f) fclose(f);
    f = fopen(KITE_HEAD, "w"); if (f) { fputs("ref: refs/heads/main\n", f); fclose(f); }
    f = fopen(KITE_REFS "/heads/main", "w"); if (f) fclose(f);
    f = fopen(KITE_REMOTE, "w"); if (f) fclose(f);

    printf("Initialized Kite repository\n");
    return 0;
}

int sys_index_add(const char *p)
{
    FILE *f = fopen(KITE_INDEX, "a");
    if (!f) return 1;

    fprintf(f, "%s\n", p);
    fclose(f);

    return 0;
}

int sys_repo_commit(const char *msg)
{
    FILE *idx = fopen(KITE_INDEX, "r");
    if (!idx) return 1;

    fseek(idx, 0, SEEK_END);
    long size = ftell(idx);
    fclose(idx);

    if (size <= 0)
    {
        printf("kite: nothing to commit\n");
        return 1;
    }

    char id[64];
    snprintf(id, sizeof(id), "%ld", (long)time(NULL));

    char path[256];
    snprintf(path, sizeof(path), ".kite/commits/%s", id);

    FILE *c = fopen(path, "w");
    if (!c) return 1;

    fprintf(c, "commit %s\n", id);
    fprintf(c, "message %s\n", msg);

    fclose(c);

    FILE *clear = fopen(KITE_INDEX, "w");
    if (clear) fclose(clear);

    printf("[%s] %s\n", id, msg);

    return 0;
}