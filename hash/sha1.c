#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    uint32_t state[5];
    uint64_t bitlen;
    unsigned char buffer[64];
    size_t buffer_len;
} sha1_ctx;

static uint32_t rotl32(uint32_t x, uint32_t n)
{
    return (x << n) | (x >> (32 - n));
}

static void sha1_init(sha1_ctx *ctx)
{
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xC3D2E1F0;
    ctx->bitlen = 0;
    ctx->buffer_len = 0;
}

static void sha1_transform(sha1_ctx *ctx, const unsigned char data[64])
{
    uint32_t w[80];

    for (int i = 0; i < 16; i++)
    {
        w[i] = (data[i * 4] << 24) |
               (data[i * 4 + 1] << 16) |
               (data[i * 4 + 2] << 8) |
               (data[i * 4 + 3]);
    }

    for (int i = 16; i < 80; i++)
        w[i] = rotl32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);

    uint32_t a = ctx->state[0];
    uint32_t b = ctx->state[1];
    uint32_t c = ctx->state[2];
    uint32_t d = ctx->state[3];
    uint32_t e = ctx->state[4];

    for (int i = 0; i < 80; i++)
    {
        uint32_t f, k;

        if (i < 20)
        {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        }
        else if (i < 40)
        {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        else if (i < 60)
        {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        }
        else
        {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }

        uint32_t temp = rotl32(a, 5) + f + e + k + w[i];
        e = d;
        d = c;
        c = rotl32(b, 30);
        b = a;
        a = temp;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
}

static void sha1_update(sha1_ctx *ctx, const unsigned char *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        ctx->buffer[ctx->buffer_len++] = data[i];
        ctx->bitlen += 8;

        if (ctx->buffer_len == 64)
        {
            sha1_transform(ctx, ctx->buffer);
            ctx->buffer_len = 0;
        }
    }
}

static void sha1_final(sha1_ctx *ctx, unsigned char hash[20])
{
    size_t i = ctx->buffer_len;

    ctx->buffer[i++] = 0x80;

    if (i > 56)
    {
        while (i < 64)
            ctx->buffer[i++] = 0;

        sha1_transform(ctx, ctx->buffer);
        i = 0;
    }

    while (i < 56)
        ctx->buffer[i++] = 0;

    uint64_t bitlen = ctx->bitlen;

    for (int j = 7; j >= 0; j--)
        ctx->buffer[56 + (7 - j)] = (bitlen >> (j * 8)) & 0xFF;

    sha1_transform(ctx, ctx->buffer);

    for (i = 0; i < 5; i++)
    {
        hash[i * 4 + 0] = (ctx->state[i] >> 24) & 0xFF;
        hash[i * 4 + 1] = (ctx->state[i] >> 16) & 0xFF;
        hash[i * 4 + 2] = (ctx->state[i] >> 8) & 0xFF;
        hash[i * 4 + 3] = (ctx->state[i]) & 0xFF;
    }
}

int sha1_compute(const void *data, size_t len, char *out_hex, size_t out_size)
{
    if (!data || !out_hex || out_size < 41)
        return 1;

    sha1_ctx ctx;
    unsigned char hash[20];

    sha1_init(&ctx);
    sha1_update(&ctx, (const unsigned char *)data, len);
    sha1_final(&ctx, hash);

    for (int i = 0; i < 20; i++)
        snprintf(out_hex + (i * 2), 3, "%02x", hash[i]);

    return 0;
}