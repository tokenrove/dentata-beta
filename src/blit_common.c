/* 
 * blit_common.c
 * Created: Mon Jan 29 13:42:41 2001 by tek@wiw.org
 * Revised: Sat May  5 16:34:27 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>
#include <dentata/error.h>

typedef void(*blitfunction_t)(byte *, byte *, byte *, byte *, dword, dword,
                              dword, dword, dword, dword);

extern void _blit80(byte *, byte *, byte *, byte *, dword, dword, dword,
                    dword, dword, dword);
extern void _blit81(byte *, byte *, byte *, byte *, dword, dword, dword,
                    dword, dword, dword);
extern void _blit160(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit161(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit162(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit164(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit168(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit240(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit241(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit242(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit244(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit248(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);

#define NBPPS   4
#define NALPHAS 5

static blitfunction_t blits[NBPPS][NALPHAS] = {
    /* 8 bpp */ { _blit80, _blit81, _blit80, _blit80, _blit80 },
    /* 15 bpp */ { 0, 0, 0, 0, 0 },
    /* 16 bpp */ { _blit160, _blit161, _blit162, _blit164, _blit168 },
    /* 24 bpp */ { _blit240, _blit241, _blit242, _blit244, _blit248 }
};

void d_image_blit(d_image_t *, d_image_t *, d_point_t);
void __d_image_clip(d_image_t *, d_image_t *, d_point_t, dword *, dword *,
                    dword *, dword *, dword *, dword *);

void d_image_blit(d_image_t *d, d_image_t *s, d_point_t p)
{
    dword dstoffset, dstscanoff, scanlen, endoffset, srcoffset, srcscanoff;
    int i, j;
    bool sdirty = false;

    if(s->desc.bpp != d->desc.bpp) {
        s = d_image_dup(s);
        d_image_convertdepth(s, d->desc.bpp);
        sdirty = true;
    }

    __d_image_clip(d, s, p, &dstoffset, &dstscanoff, &scanlen, &endoffset,
                   &srcoffset, &srcscanoff);
    switch(d->desc.bpp) {
    case 8:
        i = 0;
        break;
    case 15:
        i = 1;
    case 16:
        i = 2;
        break;
    case 24:
        i = 3;
        break;
    default:
        d_error_fatal("d_image_blit: Unsupported bpp.");
        return;
    }

    switch(s->desc.alpha) {
    case 0:
        j = 0;
        break;
    case 1:
        j = 1;
        break;
    case 2:
        j = 2;
        break;
    case 4:
        j = 3;
        break;
    case 8:
        j = 4;
        break;
    default:
        d_error_fatal("d_image_blit: Unsupported alpha.");
        break;
    }

    if(dstoffset < d->desc.w*d->desc.h && scanlen > 0 &&
       dstoffset < endoffset)
        (*blits[i][j])(d->data, d->alpha, s->data, s->alpha, dstoffset,
                       dstscanoff, scanlen, endoffset, srcoffset, srcscanoff);

    if(sdirty)
        d_image_delete(s);

    return;
}

#define max(a, b) (((a) > (b))?(a):(b))
#define min(a, b) (((a) > (b))?(b):(a))

void __d_image_clip(d_image_t *d, d_image_t *s, d_point_t p, dword *dstoffset,
                    dword *dstscanoff, dword *scanlen, dword *endoffset,
                    dword *srcoffset, dword *srcscanoff)
{
    int maxx, maxy, i;

    maxy = max(0, -p.y);
    maxx = max(0, -p.x);
    *srcoffset = s->desc.w*maxy+maxx;
    *dstoffset = d->desc.w*(maxy+p.y)+(maxx+p.x);
    *scanlen = max(0, min(s->desc.w-maxx, d->desc.w-(p.x+maxx)));
    *dstscanoff = d->desc.w-(*scanlen+(p.x+maxx))+(p.x+maxx);
    *srcscanoff = s->desc.w-(*scanlen+(p.x+maxx))+(p.x+maxx);
    i = max(0, min(s->desc.h-maxy, d->desc.h-(maxy+p.y)));
    *endoffset = *dstoffset+(*dstscanoff+*scanlen)*i;
    return;
}

/* EOF blit_common.c */
