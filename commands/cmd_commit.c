#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define KITE_INDEX ".kite/index"
#define KITE_HEAD ".kite/HEAD"
#define KITE_COMMITS ".kite/commits"

static uint64_t fnv1a_string(const char *str)
{
    uint64_t hash = 1469598103934665603ULL;

    while (*str)
    {
        hash ^= (unsigned char)*str++;
        hash *= 1099511628211ULL;
    }

    return hash;
}

static void generate_commit_id(char *buffer, size_t size)
{
    char seed[512];
    time_t now = time(NULL);

    snprintf(seed,
             sizeof(seed),
             "%lld-%ld-%u",
             (long long)now,
             (long)clock(),
             (unsigned)rand());

    snprintf(buffer,
             size,
             "%016llx",
             (unsigned long long)fnv1a_string(seed));
}

static int get_head_ref(char *buffer, size_t size)
{
    FILE *fp = fopen(KITE_HEAD, "r");

    if (!fp)
        return 1;

    if (!fgets(buffer, (int)size, fp))
    {
        fclose(fp);
        return 1;
    }

    fclose(fp);

    if (strncmp(buffer, "ref:", 4) == 0)
    {
        memmove(buffer, buffer + 4, strlen(buffer + 4) + 1);

        while (*buffer == ' ')
            memmove(buffer, buffer + 1, strlen(buffer));
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';

    return 0;
}

static int get_current_branch(char *buffer, size_t size)
{
    char ref[512];

    if (get_head_ref(ref, sizeof(ref)) != 0)
        return 1;

    const char *name = strrchr(ref, '/');

    if (!name)
        return 1;

    name++;

    strncpy(buffer, name, size - 1);
    buffer[size - 1] = '\0';

    return 0;
}

static int update_branch_ref(const char *commit_id)
{
    char ref[512];
    char path[1024];

    if (get_head_ref(ref, sizeof(ref)) != 0)
        return 1;

    snprintf(path,
             sizeof(path),
             ".kite/%s",
             ref);

    FILE *fp = fopen(path, "w");

    if (!fp)
        return 1;

    fprintf(fp, "%s\n", commit_id);

    fclose(fp);

    return 0;
}

static int write_commit_object(const char *commit_id,
                               const char *message)
{
    char commit_path[1024];
    char branch[256];
    char timestamp[128];

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    strftime(timestamp,
             sizeof(timestamp),
             "%Y-%m-%d %H:%M:%S",
             tm_info);

    get_current_branch(branch, sizeof(branch));

    snprintf(commit_path,
             sizeof(commit_path),
             "%s/%s.kite",
             KITE_COMMITS,
             commit_id);

    FILE *commit = fopen(commit_path, "w");

    if (!commit)
        return 1;

    fprintf(commit, "commit %s\n", commit_id);
    fprintf(commit, "branch %s\n", branch);
    fprintf(commit, "date %s\n", timestamp);
    fprintf(commit, "message %s\n\n", message);

    FILE *index = fopen(KITE_INDEX, "r");

    if (index)
    {
        char line[4096];

        while (fgets(line, sizeof(line), index))
            fputs(line, commit);

        fclose(index);
    }

    fclose(commit);

    return 0;
}

int cmd_commit(int argc, char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr,
                "usage: kite commit -m \"message\"\n");
        return 1;
    }

    if (strcmp(argv[2], "-m") != 0)
    {
        fprintf(stderr,
                "usage: kite commit -m \"message\"\n");
        return 1;
    }

    const char *message = argv[3];

    FILE *index = fopen(KITE_INDEX, "r");

    if (!index)
    {
        fprintf(stderr,
                "kite: nothing staged\n");
        return 1;
    }

    fseek(index, 0, SEEK_END);

    long size = ftell(index);

    fclose(index);

    if (size <= 0)
    {
        fprintf(stderr,
                "kite: nothing staged\n");
        return 1;
    }

    char commit_id[64];

    generate_commit_id(commit_id,
                       sizeof(commit_id));

    if (write_commit_object(commit_id,
                            message) != 0)
    {
        fprintf(stderr,
                "kite: failed to create commit\n");
        return 1;
    }

    if (update_branch_ref(commit_id) != 0)
    {
        fprintf(stderr,
                "kite: failed to update branch\n");
        return 1;
    }

    FILE *clear = fopen(KITE_INDEX, "w");

    if (clear)
        fclose(clear);

    printf("[%s] %s\n",
           commit_id,
           message);

    return 0;
}