/* 
 * collide_common.c
 * Created: Sat Apr 28 16:31:04 2001 by tek@wiw.org
 * Revised: Fri May  4 11:57:07 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>
#include <dentata/error.h>

#include "internal.h"

#define NCOLLIDES 10

typedef bool (*collidefunction_t)(byte *, byte *, dword, dword,
                                  dword, dword, dword, dword);

bool __collide_bounding(byte *, byte *, dword, dword, dword, dword,
                        dword, dword);
bool __collide_pixel1(byte *, byte *, dword, dword, dword, dword,
                      dword, dword);
bool __collide_pixel2(byte *, byte *, dword, dword, dword, dword,
                      dword, dword);
bool __collide_pixel4(byte *, byte *, dword, dword, dword, dword,
                      dword, dword);
bool __collide_pixel8(byte *, byte *, dword, dword, dword, dword,
                      dword, dword);
bool __collide_thresh2(byte *, byte *, dword, dword, dword, dword,
                       dword, dword);
bool __collide_thresh4(byte *, byte *, dword, dword, dword, dword,
                       dword, dword);
bool __collide_thresh8(byte *, byte *, dword, dword, dword, dword,
                       dword, dword);

static collidefunction_t collides[NCOLLIDES] = {
  __collide_bounding, __collide_pixel1, __collide_pixel2, __collide_pixel4,
  __collide_pixel8,
  __collide_bounding, __collide_pixel1, __collide_thresh2, __collide_thresh4,
  __collide_thresh8
};

bool d_image_collide(d_image_t *a, d_image_t *b, d_point_t p,
                     d_collidemode_t cmode)
{
    dword aoffset, ascanoff, scanlen, endoffset, boffset, bscanoff;
    int i, j;
    bool adirty = false, ret;

    __d_image_clip(a, b, p, &aoffset, &ascanoff, &scanlen, &endoffset,
                   &boffset, &bscanoff);
    switch(b->desc.alpha) {
    case 0:
    case 1:
    case 2:
        j = b->desc.alpha;
        break;
    case 4:
        j = 3;
        break;
    case 8:
        j = 4;
        break;
    default:
        d_error_push("d_image_collide: unsupported alpha.");
        j = 0;
        break;
    }
    switch(cmode) {
    case bounding:
        j = 0;
        break;
    case alphathresh:
        j += NCOLLIDES/2;
    case pixel:
    default:
        break;
    }

    /* FIXME, should go to whichever is higher */
    if((j%(NCOLLIDES/2)) != 0 && a->desc.alpha != b->desc.alpha) {
        /* FIXME this error should only occur if debug is switched on */
        d_error_fatal("d_image_collide: Conversion (%d -> %d) occurred.\n",
                      a->desc.alpha, b->desc.alpha);
        a = d_image_dup(a);
        d_image_extendalpha(a, b->desc.alpha);
        adirty = true;
    }

    if(aoffset < a->desc.w*a->desc.h && scanlen > 0 &&
       aoffset < endoffset)
        ret = (*collides[j])(a->alpha, b->alpha, aoffset, ascanoff, scanlen,
                             endoffset, boffset, bscanoff);
    else
        ret = false;

    if(adirty)
        d_image_delete(a);

    return ret;
}

/* EOF collide_common.c */
