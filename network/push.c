#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/network.h"
#include "../include/protocol.h"

int net_server_listen(int port)
{
    (void)port;

    FILE *f = fopen(".kite/server.log", "a");
    if (!f)
        return 1;

    fprintf(f, "Server started on port %d\n", port);
    fclose(f);

    return 0;
}

int net_server_receive(const char *packet, char **out_response)
{
    if (!packet || !out_response)
        return 1;

    FILE *f = fopen(".kite/server.log", "a");
    if (!f)
        return 1;

    fprintf(f, "RECV: %s\n", packet);
    fclose(f);

    char *response = malloc(256);
    if (!response)
        return 1;

    if (strstr(packet, "PUSH"))
        snprintf(response, 256, "ACK PUSH");
    else if (strstr(packet, "FETCH"))
        snprintf(response, 256, "ACK FETCH");
    else
        snprintf(response, 256, "UNKNOWN OP");

    *out_response = response;

    return 0;
}

int net_server_broadcast(const char *msg)
{
    if (!msg)
        return 1;

    FILE *f = fopen(".kite/server.log", "a");
    if (!f)
        return 1;

    fprintf(f, "BROADCAST: %s\n", msg);
    fclose(f);

    return 0;
}