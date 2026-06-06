#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KITE_HEAD ".kite/HEAD"
#define KITE_REFS ".kite/refs/heads"
#define KITE_COMMITS ".kite/commits"

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

static int is_commit_valid(const char *id)
{
    char path[512];
    snprintf(path, sizeof(path), "%s/%s.kite", KITE_COMMITS, id);

    FILE *fp = fopen(path, "r");
    if (!fp)
        return 0;

    fclose(fp);
    return 1;
}

static int set_branch_commit(const char *branch, const char *commit)
{
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", KITE_REFS, branch);

    FILE *fp = fopen(path, "w");
    if (!fp)
        return 1;

    fprintf(fp, "%s\n", commit);
    fclose(fp);

    return 0;
}

static int get_head_commit(char *out, size_t size)
{
    char branch[256];
    if (get_branch(branch, sizeof(branch)) != 0)
        return 1;

    return get_commit(branch, out, size);
}

static int write_merge_commit(const char *branch, const char *other, const char *message)
{
    char head[256];
    if (get_head_commit(head, sizeof(head)) != 0)
        head[0] = 0;

    char new_id[64];
    snprintf(new_id, sizeof(new_id), "merge_%ld", (long)rand());

    char path[512];
    snprintf(path, sizeof(path), "%s/%s.kite", KITE_COMMITS, new_id);

    FILE *fp = fopen(path, "w");
    if (!fp)
        return 1;

    fprintf(fp, "commit %s\n", new_id);
    fprintf(fp, "type merge\n");
    fprintf(fp, "branch %s\n", branch);
    fprintf(fp, "merge %s\n", other);
    fprintf(fp, "parent %s\n", head);
    fprintf(fp, "message %s\n\n", message);

    fclose(fp);

    return 0;
}

int cmd_merge(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: kite merge <branch>\n");
        return 1;
    }

    char current[256];
    if (get_branch(current, sizeof(current)) != 0)
    {
        fprintf(stderr, "kite: cannot resolve current branch\n");
        return 1;
    }

    char target[256];
    strncpy(target, argv[2], sizeof(target) - 1);
    target[sizeof(target) - 1] = 0;

    if (!is_commit_valid(target))
    {
        fprintf(stderr, "kite: branch/commit '%s' not found\n", target);
        return 1;
    }

    char base_commit[256];
    if (get_commit(target, base_commit, sizeof(base_commit)) != 0)
    {
        fprintf(stderr, "kite: cannot read target branch\n");
        return 1;
    }

    if (write_merge_commit(current, target, "merge commit") != 0)
    {
        fprintf(stderr, "kite: merge failed\n");
        return 1;
    }

    char new_id[64];
    snprintf(new_id, sizeof(new_id), "merge_%ld", (long)rand());

    if (set_branch_commit(current, new_id) != 0)
    {
        fprintf(stderr, "kite: failed to update branch\n");
        return 1;
    }

    printf("Merged %s into %s\n", target, current);

    return 0;
}