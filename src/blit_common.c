/* 
 * blit_common.c
 * Created: Mon Jan 29 13:42:41 2001 by tek@wiw.org
 * Revised: Wed Apr 11 08:21:08 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/blit.h>

typedef void(*blitfunction_t)(byte *, byte *, byte *, byte *, dword, dword,
                              dword, dword, dword, dword);

extern void _blit80(byte *, byte *, byte *, byte *, dword, dword, dword,
                    dword, dword, dword);
extern void _blit160(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit161(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit162(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);

static blitfunction_t blits[4][5] = {
    /* 8 bpp */ { _blit80, 0, 0, 0, 0 },
    /* 15 bpp */ { 0, 0, 0, 0, 0 },
    /* 16 bpp */ { _blit160, _blit161, _blit162, 0, 0 },
    /* 24 bpp */ { 0, 0, 0, 0, 0 }
//    /*  8 bpp */ { _blit80,  _blit81,  _blit82,  _blit84,  _blit88 },
//    /* 15 bpp */ { _blit150, _blit151, _blit152, _blit154, _blit158 },
//    /* 16 bpp */ { _blit160, _blit161, _blit162, _blit164, _blit168 },
//    /* 24 bpp */ { _blit240, _blit241, _blit242, _blit244, _blit248 }
};

void d_blit(d_image_t *, d_image_t *, d_point_t);
static void clip(d_image_t *, d_image_t *, d_point_t, dword *, dword *,
                 dword *, dword *, dword *, dword *);

void d_blit(d_image_t *d, d_image_t *s, d_point_t p)
{
    dword dstoffset, dstscanoff, scanlen, endoffset, srcoffset, srcscanoff;
    int i, j;

    clip(d, s, p, &dstoffset, &dstscanoff, &scanlen, &endoffset,
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
    }

    switch(d->desc.alpha) {
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
    }
    (*blits[i][j])(d->data, d->alpha, s->data, s->alpha, dstoffset,
                   dstscanoff, scanlen, endoffset, srcoffset, srcscanoff);
    return;
}

#define max(a, b) (((a) > (b))?(a):(b))
#define min(a, b) (((a) > (b))?(b):(a))

static void clip(d_image_t *d, d_image_t *s, d_point_t p, dword *dstoffset,
                 dword *dstscanoff, dword *scanlen, dword *endoffset,
                 dword *srcoffset, dword *srcscanoff)
{
    int maxx, maxy, i;

    maxy = max(0, -p.y);
    maxx = max(0, -p.x);
    *srcoffset = s->desc.w*maxy+maxx;
    *dstoffset = d->desc.w*(maxy+p.y)+(maxx+p.x);
    *scanlen = min(s->desc.w-maxx, d->desc.w-(p.x+maxx));
    *dstscanoff = d->desc.w-(*scanlen+p.x)+maxx+p.x;
    *srcscanoff = s->desc.w-*scanlen+maxx;
    i = min(s->desc.h-maxy, s->desc.h-(maxy+p.y));
    *endoffset = *dstoffset+(*dstscanoff+*scanlen)*i;
    return;
}

/* EOF blit_common.c */
