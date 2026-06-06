#ifndef KITE_PROTOCOL_H
#define KITE_PROTOCOL_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KITE_PROTO_VERSION 1

typedef enum kite_proto_op {
    KITE_OP_PUSH,
    KITE_OP_FETCH,
    KITE_OP_CLONE,
    KITE_OP_UPDATE,
    KITE_OP_ACK
} kite_proto_op;

typedef struct kite_packet {
    kite_proto_op op;
    char *payload;
    size_t size;
} kite_packet;

int proto_encode(const kite_packet *pkt, char **out, size_t *out_size);
int proto_decode(const char *data, size_t size, kite_packet *out);
void proto_free(kite_packet *pkt);

#ifdef __cplusplus
}
#endif

#endif