/* 
 * alphacomp_generic.c
 * Created: Sat May 19 12:28:33 2001 by tek@wiw.org
 * Revised: Sat May 19 12:29:21 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>

#include "internal.h"

void d_image_alphacomp(d_image_t *d, d_image_t *s, d_point_t p);

static void __d_image_alphacomp_fast(byte *, byte *, byte, dword, dword,
                                     dword, dword, dword, dword);
static void __d_image_alphacomp_slow(byte *, byte *, byte, byte,
                                     dword, dword, dword, dword, dword, dword);

#define min(a, b) (((a) > (b))?(b):(a))

void d_image_alphacomp(d_image_t *d, d_image_t *s, d_point_t p)
{
    dword dstoffset, dstscanoff, scanlen, endoffset, srcoffset, srcscanoff;

    if(d->alpha == 0 || s->alpha == 0)
        return;

    __d_image_clip(d, s, p, &dstoffset, &dstscanoff, &scanlen, &endoffset,
                   &srcoffset, &srcscanoff);
    if(dstoffset < d->desc.w*d->desc.h && scanlen > 0 &&
       dstoffset < endoffset) {
        if(d->desc.alpha == s->desc.alpha) {
            __d_image_alphacomp_fast(d->alpha, s->alpha, d->desc.alpha,
                                     dstoffset, dstscanoff, scanlen,
                                     endoffset, srcoffset, srcscanoff);
        } else {
            __d_image_alphacomp_slow(d->alpha, s->alpha, d->desc.alpha,
                                     s->desc.alpha, dstoffset, dstscanoff,
                                     scanlen, endoffset, srcoffset,
                                     srcscanoff);
        }
    }

    return;
}

void __d_image_alphacomp_slow(byte *d, byte *s, byte d_alpha, byte s_alpha,
                              dword dstoffset, dword dstscanoff,
                              dword scanlen, dword endoffset,
                              dword srcoffset, dword srcscanoff)
{
    byte d_mask, s_mask, m, o, a_s, *end;
    dword a, b, j;

    d_mask = (1<<d_alpha)-1;
    s_mask = (1<<s_alpha)-1;
    o = 8/d_alpha;
    m = 8/s_alpha;
    end = d+endoffset/o;
    d += dstoffset/o;
    a = dstoffset%o;
    s += srcoffset/m;
    b = srcoffset%m;

    do {
        for(j = scanlen; j > 0; j--) {
            a_s = ((*s)>>(s_alpha*(b%m)))&s_mask;
            if(s_alpha > d_alpha) {
                a_s >>= (s_alpha-d_alpha);
            } else {
                a_s <<= (d_alpha-s_alpha);
            }
            a_s += ((*d)>>(d_alpha*(a%o)))&d_mask;
            *d &= ~(d_mask<<(d_alpha*(a%o)));
            *d |= min(a_s, d_mask)<<(d_alpha*(a%o));
            a++; b++;
            if(a == o) {
                a = 0; d++;
            }
            if(b == m) {
                b = 0; s++;
            }
        }

        d += dstscanoff/o;
        s += srcscanoff/m;
        a += dstscanoff%o;
        b += srcscanoff%m;
        if(a >= o) {
            a %= o;
            d++;
        }
        if(b >= m) {
            b %= m;
            s++;
        }
    } while(d < end);

    return;
}

void __d_image_alphacomp_fast(byte *d, byte *s, byte alpha, dword dstoffset,
                              dword dstscanoff, dword scanlen, dword endoffset,
                              dword srcoffset, dword srcscanoff)
{
    byte mask, o, a_s, a_d;
    dword a, b, i, j;

    mask = (1<<alpha)-1;
    o = 8/alpha;

    for(i = 0; dstoffset+i*(scanlen+dstscanoff)+scanlen < endoffset; i++) {
        for(j = 0; j < scanlen; j++) {
            a = dstoffset+i*(scanlen+dstscanoff)+j;
            b = srcoffset+i*(scanlen+srcscanoff)+j;
            a_d = (d[a/o]>>(alpha*(a%o)))&mask;
            a_s = (s[b/o]>>(alpha*(b%o)))&mask;
            d[a/o] &= ~(mask<<(alpha*(a%o)));
            d[a/o] |= min(a_s+a_d, mask)<<(alpha*(a%o));
        }
    }

    return;
}

/* EOF alphacomp_generic.c */
