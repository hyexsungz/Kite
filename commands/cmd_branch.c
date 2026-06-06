#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

#define KITE_HEAD ".kite/HEAD"
#define KITE_REFS ".kite/refs/heads"

static int file_exists(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp)
    {
        fclose(fp);
        return 1;
    }
    return 0;
}

static int create_branch(const char *name)
{
    char head_ref[1024] = {0};
    char current_commit[256] = {0};
    char branch_path[1024];

    FILE *head = fopen(KITE_HEAD, "r");
    if (!head)
        return 1;

    fgets(head_ref, sizeof(head_ref), head);
    fclose(head);

    char *ref = strstr(head_ref, "ref:");
    if (!ref)
        return 1;

    ref += 4;
    while (*ref == ' ')
        ref++;

    ref[strcspn(ref, "\r\n")] = 0;

    char ref_path[1024];
    snprintf(ref_path, sizeof(ref_path), ".kite/%s", ref);

    FILE *ref_fp = fopen(ref_path, "r");
    if (ref_fp)
    {
        fgets(current_commit, sizeof(current_commit), ref_fp);
        fclose(ref_fp);
    }

    snprintf(branch_path,
             sizeof(branch_path),
             "%s/%s",
             KITE_REFS,
             name);

    if (file_exists(branch_path))
        return 1;

    FILE *branch = fopen(branch_path, "w");
    if (!branch)
        return 1;

    fputs(current_commit, branch);
    fclose(branch);

    return 0;
}

static int current_branch(char *buffer, size_t size)
{
    FILE *fp = fopen(KITE_HEAD, "r");
    if (!fp)
        return 1;

    char line[1024];

    if (!fgets(line, sizeof(line), fp))
    {
        fclose(fp);
        return 1;
    }

    fclose(fp);

    char *slash = strrchr(line, '/');
    if (!slash)
        return 1;

    slash++;

    strncpy(buffer, slash, size - 1);
    buffer[size - 1] = '\0';

    buffer[strcspn(buffer, "\r\n")] = 0;

    return 0;
}

#ifdef _WIN32

#include <windows.h>

static int list_branches(void)
{
    char active[256] = {0};
    current_branch(active, sizeof(active));

    char pattern[1024];
    snprintf(pattern, sizeof(pattern), "%s\\*", KITE_REFS);

    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern, &fd);

    if (h == INVALID_HANDLE_VALUE)
        return 1;

    do
    {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        if (strcmp(fd.cFileName, active) == 0)
            printf("* %s\n", fd.cFileName);
        else
            printf("  %s\n", fd.cFileName);

    } while (FindNextFileA(h, &fd));

    FindClose(h);
    return 0;
}

#else

#include <dirent.h>

static int list_branches(void)
{
    char active[256] = {0};
    current_branch(active, sizeof(active));

    DIR *dir = opendir(KITE_REFS);
    if (!dir)
        return 1;

    struct dirent *ent;

    while ((ent = readdir(dir)) != NULL)
    {
        if (strcmp(ent->d_name, ".") == 0)
            continue;

        if (strcmp(ent->d_name, "..") == 0)
            continue;

        if (strcmp(ent->d_name, active) == 0)
            printf("* %s\n", ent->d_name);
        else
            printf("  %s\n", ent->d_name);
    }

    closedir(dir);
    return 0;
}

#endif

int cmd_branch(int argc, char **argv)
{
    if (argc == 2)
        return list_branches();

    if (argc == 3)
        return create_branch(argv[2]);

    return 1;
}