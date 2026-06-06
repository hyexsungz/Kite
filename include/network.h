#ifndef KITE_NETWORK_H
#define KITE_NETWORK_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kite_response {
    int status_code;
    char *body;
    size_t size;
} kite_response;

int net_init(void);
int net_get(const char *url, kite_response *out);
int net_post(const char *url, const char *data, kite_response *out);
int net_push(const char *remote, const char *payload);
int net_fetch(const char *remote, kite_response *out);
void net_free_response(kite_response *res);

#ifdef __cplusplus
}
#endif

#endif