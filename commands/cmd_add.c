#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define PATH_SEP '\\'
#else
#include <dirent.h>
#include <unistd.h>
#define PATH_SEP '/'
#endif

#define KITE_INDEX ".kite/index"
#define BUFFER_SIZE 8192

static uint64_t kite_hash_file(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp)
        return 0;

    uint64_t hash = 1469598103934665603ULL;
    unsigned char buffer[BUFFER_SIZE];
    size_t n;

    while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
    {
        for (size_t i = 0; i < n; i++)
        {
            hash ^= buffer[i];
            hash *= 1099511628211ULL;
        }
    }

    fclose(fp);
    return hash;
}

static int is_directory(const char *path)
{
    struct stat st;

    if (stat(path, &st) != 0)
        return 0;

    return S_ISDIR(st.st_mode);
}

static int is_kite_internal(const char *path)
{
    return strstr(path, ".kite") != NULL;
}

static int index_contains(const char *path)
{
    FILE *fp = fopen(KITE_INDEX, "r");

    if (!fp)
        return 0;

    char line[4096];

    while (fgets(line, sizeof(line), fp))
    {
        char indexed[4096];

        if (sscanf(line, "%*s %4095[^\n]", indexed) == 1)
        {
            if (strcmp(indexed, path) == 0)
            {
                fclose(fp);
                return 1;
            }
        }
    }

    fclose(fp);
    return 0;
}

static int append_index(const char *path)
{
    if (index_contains(path))
        return 0;

    FILE *fp = fopen(KITE_INDEX, "a");

    if (!fp)
        return -1;

    fprintf(fp, "%016llx %s\n",
            (unsigned long long)kite_hash_file(path),
            path);

    fclose(fp);
    return 0;
}

#ifdef _WIN32

static int add_directory(const char *dirpath)
{
    char pattern[4096];
    snprintf(pattern, sizeof(pattern), "%s\\*", dirpath);

    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern, &fd);

    if (h == INVALID_HANDLE_VALUE)
        return -1;

    do
    {
        if (strcmp(fd.cFileName, ".") == 0 ||
            strcmp(fd.cFileName, "..") == 0)
            continue;

        char fullpath[4096];

        snprintf(fullpath,
                 sizeof(fullpath),
                 "%s\\%s",
                 dirpath,
                 fd.cFileName);

        if (is_kite_internal(fullpath))
            continue;

        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            add_directory(fullpath);
        }
        else
        {
            append_index(fullpath);
        }

    } while (FindNextFileA(h, &fd));

    FindClose(h);
    return 0;
}

#else

static int add_directory(const char *dirpath)
{
    DIR *dir = opendir(dirpath);

    if (!dir)
        return -1;

    struct dirent *ent;

    while ((ent = readdir(dir)) != NULL)
    {
        if (strcmp(ent->d_name, ".") == 0 ||
            strcmp(ent->d_name, "..") == 0)
            continue;

        char fullpath[4096];

        snprintf(fullpath,
                 sizeof(fullpath),
                 "%s/%s",
                 dirpath,
                 ent->d_name);

        if (is_kite_internal(fullpath))
            continue;

        if (is_directory(fullpath))
        {
            add_directory(fullpath);
        }
        else
        {
            append_index(fullpath);
        }
    }

    closedir(dir);
    return 0;
}

#endif

int cmd_add(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "kite add <file|directory>\n");
        return 1;
    }

    for (int i = 2; i < argc; i++)
    {
        const char *target = argv[i];

        if (is_kite_internal(target))
            continue;

        if (is_directory(target))
        {
            add_directory(target);
        }
        else
        {
            append_index(target);
        }
    }

    return 0;
}