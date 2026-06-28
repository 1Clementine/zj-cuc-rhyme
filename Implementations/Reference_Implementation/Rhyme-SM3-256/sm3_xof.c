/* SM3-based eXtensible Output Function state machine.
 *
 * Internally uses API_PKC's pseudoXOF() (SM3-KDF) and sm3hash().
 * Two modes:
 *   XOF128  (mode=0) — for Agen expansion / sampling streams
 *   XOF256  (mode=1) — for hashing mu, rhoprime, c_tilde, keygen seed
 *
 * The incremental API buffers all absorb() calls; the actual SM3-KDF
 * call is deferred until the first squeeze(), at which point we
 * prepend a domain-separation tag byte and call pseudoXOF once for
 * the entire output length needed.
 *
 * This is a Rhyme-specific thin wrapper built on the API_PKC auxiliary-
 * function layer (auxfunc.h / auxfunc.c). */

#include "sm3_xof.h"
#include "auxfunc.h"
#include <stdlib.h>
#include <string.h>

enum { TAG_XOF128 = 0x01, TAG_XOF256 = 0x02 };

/* ------------------------------------------------------------------ helpers */

static void state_reset(sm3_xof_state *s, uint8_t mode)
{
    memset(s, 0, sizeof(*s));
    s->mode = mode;
}

static void state_absorb(sm3_xof_state *s, const uint8_t *in, size_t inlen)
{
    if (s->overflow || s->finalized || inlen == 0)
        return;
    if (s->inlen + inlen > sizeof(s->input)) {
        s->overflow = 1;
        return;
    }
    memcpy(s->input + s->inlen, in, inlen);
    s->inlen += inlen;
}

/* Call pseudoXOF with a tag-byte prefix, return 0 on success. */
static int tagged_xof(uint8_t *out, size_t outlen, uint8_t tag,
                      const uint8_t *in, size_t inlen)
{
    size_t tagged_len = inlen + 1;
    uint8_t *buf = (uint8_t *)malloc(tagged_len);
    if (!buf) return -1;

    buf[0] = tag;
    if (inlen > 0) memcpy(buf + 1, in, inlen);

    (void)pseudoXOF((unsigned long long)(outlen * 8ULL),
                    buf, (unsigned long long)(tagged_len * 8ULL), out);

    free(buf);
    return 0;
}

static void state_squeeze(uint8_t *out, size_t outlen, sm3_xof_state *s)
{
    if (outlen == 0) return;
    if (s->overflow) { memset(out, 0, outlen); return; }

    /* total bytes we need from this XOF stream */
    size_t total = s->outpos + outlen;
    uint8_t *tmp = (uint8_t *)malloc(total);
    if (!tmp) { memset(out, 0, outlen); return; }

    uint8_t tag = (s->mode == 0) ? TAG_XOF128 : TAG_XOF256;
    if (tagged_xof(tmp, total, tag, s->input, s->inlen) != 0) {
        memset(out, 0, outlen);
        free(tmp);
        return;
    }
    memcpy(out, tmp + s->outpos, outlen);
    s->outpos += outlen;
    free(tmp);
}

/* ------------------------------------------------------------------ public */

/* --- XOF128: one-shot absorb + squeezeblocks --- */

void sm3_xof128_init(sm3_xof_state *s)
{
    state_reset(s, 0);
}
void sm3_xof128_absorb(sm3_xof_state *s, const uint8_t *in, size_t inlen)
{
    state_absorb(s, in, inlen);
}
void sm3_xof128_finalize(sm3_xof_state *s)
{
    s->finalized = 1;
}
void sm3_xof128_absorb_once(sm3_xof_state *s, const uint8_t *in, size_t inlen)
{
    sm3_xof128_init(s);
    sm3_xof128_absorb(s, in, inlen);
    sm3_xof128_finalize(s);
}
void sm3_xof128_squeezeblocks(uint8_t *out, size_t nblocks, sm3_xof_state *s)
{
    state_squeeze(out, nblocks * SM3_XOF128_RATE, s);
}

/* --- XOF256: one-shot --- */

void sm3_xof256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen)
{
    if (tagged_xof(out, outlen, TAG_XOF256, in, inlen) != 0)
        memset(out, 0, outlen);
}

/* --- XOF256 incremental API --- */

void sm3_xof256_init(sm3_xof_state *s)
{
    state_reset(s, 1);
}
void sm3_xof256_absorb(sm3_xof_state *s, const uint8_t *in, size_t inlen)
{
    state_absorb(s, in, inlen);
}
void sm3_xof256_finalize(sm3_xof_state *s)
{
    s->finalized = 1;
}
void sm3_xof256_squeeze(uint8_t *out, size_t outlen, sm3_xof_state *s)
{
    state_squeeze(out, outlen, s);
}
