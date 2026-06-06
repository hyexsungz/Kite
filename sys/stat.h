#ifndef KITE_STAT_H
#define KITE_STAT_H

#ifdef _WIN32

#include <windows.h>

struct stat {
    unsigned long st_mode;
    long long st_size;
};

static int stat(const char *path, struct stat *buf)
{
    WIN32_FILE_ATTRIBUTE_DATA data;

    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &data))
        return -1;

    buf->st_size =
        ((long long)data.nFileSizeHigh << 32) |
        data.nFileSizeLow;

    buf->st_mode = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    ? 1 : 0;

    return 0;
}

#define S_ISDIR(m) ((m) == 1)

#else

#include <sys/stat.h>

#endif

#endif