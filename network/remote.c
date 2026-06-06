#include <string.h>

int remote_get_url(const char *name, char *out, size_t size)
{
    if (!name || !out || size == 0)
        return 1;

    strncpy(out, "./kite_remote", size - 1);
    out[size - 1] = '\0';

    return 0;
}