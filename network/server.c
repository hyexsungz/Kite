#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/network.h"
#include "../include/protocol.h"

int net_init(void)
{
    FILE *f = fopen(".kite/network.state", "w");
    if (!f)
        return 1;

    fprintf(f, "network=active\n");
    fclose(f);

    return 0;
}

int net_status(char *out, size_t size)
{
    if (!out)
        return 1;

    FILE *f = fopen(".kite/network.state", "r");
    if (!f)
    {
        snprintf(out, size, "network=inactive");
        return 0;
    }

    fgets(out, size, f);
    fclose(f);

    return 0;
}

int net_send_packet(const char *packet)
{
    if (!packet)
        return 1;

    FILE *f = fopen(".kite/network.out", "a");
    if (!f)
        return 1;

    fprintf(f, "%s\n", packet);
    fclose(f);

    return 0;
}

int net_receive_packet(char *out, size_t size)
{
    FILE *f = fopen(".kite/network.out", "r");
    if (!f)
        return 1;

    if (!fgets(out, size, f))
    {
        fclose(f);
        return 1;
    }

    fclose(f);
    return 0;
}

int net_reset(void)
{
    FILE *f = fopen(".kite/network.out", "w");
    if (f)
        fclose(f);

    f = fopen(".kite/server.log", "w");
    if (f)
        fclose(f);

    return 0;
}