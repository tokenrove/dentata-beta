/* 
 * rgb.c
 * Created: Wed Jan 31 13:08:59 2001 by tek@wiw.org
 * Revised: Wed Jan 31 15:12:20 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/color.h>
#include <dentata/error.h>

d_color_t d_color_fromrgb(d_image_t *, byte, byte, byte);
d_color_t d_color_frompackedrgb(d_image_t *, dword);

static void getclosestinpalette(d_image_t *, byte *, byte *, byte *,
                                d_color_t *);

d_color_t d_color_fromrgb(d_image_t *p, byte r, byte g, byte b)
{
    d_color_t c;

    if(p == NULL) {
        d_error_push("d_color_fromrgb: Passed a NULL pointer");
        return 0;
    }

    if(p->desc.paletted == true) {
        getclosestinpalette(p, &r, &g, &b, &c);
    }

    /* FIXME for paletted non-8bpp images, is it correct to pack
       the RGB, or should we be returning the color index like we do
       for 8bpp? */
    switch(p->desc.bpp) {
    case 16: /* BBBBBGGG GGGRRRRR */
        c = 0;
        c |= r&((1<<6)-1);
        c |= (g&((1<<7)-1))<<5;
        c |= (b&((1<<6)-1))<<11;
        break;
    case 24: /* B G R */
        c = 0;
        c |= r;
        c |= g << 8;
        c |= b << 16;
        break;
    case 8: /* we only support paletted 8bpp, so we don't do anything
               to the value we grabbed from the palette above (it's not
               RGB) */
        break;

    case 15: /* MBBBBBGG GGGRRRRR I think? */
    default:
        c = 0;
        d_error_push("d_color_fromrgb: Attempted to convert with an "
                     "unsupported depth.");
    }

    return c;
}

d_color_t d_color_frompackedrgb(d_image_t *p, dword rgb)
{
    return d_color_fromrgb(p, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF);
}

#define abs(x) ((x)>0?(x):-(x))

static void getclosestinpalette(d_image_t *p, byte *r, byte *g, byte *b,
                                d_color_t *c)
{
    int i, rp, gp, bp, xbar, ybar;

    /* Here we find the closest value in the palette, and set
       c to the palette index of that value, while r, g, and b
       are set to the appropriate RGB values that are in the palette */
    i = *c = 0;
    rp = *r; gp = *g; bp = *b;
    *r = p->palette.clut[D_BYTESPERCOLOR*i+0];
    *g = p->palette.clut[D_BYTESPERCOLOR*i+1];
    *b = p->palette.clut[D_BYTESPERCOLOR*i+2];

    for(i = 0; i < D_NCLUTITEMS; i++) {
        xbar = abs(rp-p->palette.clut[D_BYTESPERCOLOR*i+0])+
               abs(gp-p->palette.clut[D_BYTESPERCOLOR*i+1])+
               abs(bp-p->palette.clut[D_BYTESPERCOLOR*i+2]);
        xbar /= 3;
        ybar = abs(*r-rp)+abs(*g-gp)+abs(*b-bp);
        ybar /= 3;

        if(xbar < ybar) {
            *c = i;
            *r = p->palette.clut[D_BYTESPERCOLOR*i+0];
            *g = p->palette.clut[D_BYTESPERCOLOR*i+1];
            *b = p->palette.clut[D_BYTESPERCOLOR*i+2];
        }
    }
    return;
}

/* EOF rgb.c */
