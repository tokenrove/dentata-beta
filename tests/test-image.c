/* 
 * test-image.c
 * Created: Thu Apr 12 05:43:22 2001 by tek@wiw.org
 * Revised: Thu Apr 19 04:08:40 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>

#define DEFW 65
#define DEFH 129
#define NBPPS 3
#define NALPHAS 5

int main(void)
{
    d_image_t *a, *b;
    d_rasterdescription_t desca, descb;
    d_point_t pt = { 0, 0 };
    int bpps[NBPPS] = { 8, 16, 24 };
    int alphas[NALPHAS] = { 0, 1, 2, 4, 8 };
    int i, j;

    desca.w = DEFW;
    desca.h = DEFH;
    descb.w = DEFW;
    descb.h = DEFH;
    desca.paletted = descb.paletted = false;
    desca.cspace = descb.cspace = RGB;

    for(i = 0; i < 2*NBPPS; i++) {
        desca.bpp = bpps[i/2];
        descb.bpp = bpps[i/2]; /* was +1, we don't support it yet because
                                  realtime dither isn't implemented */
        for(j = 0; j < 2*NALPHAS; j++) {
            desca.alpha = alphas[j/2];
            descb.alpha = alphas[j/2+1];
            a = d_image_new(desca);
            d_image_wipe(a, 0, 0);
            b = d_image_dup(a); 
            d_image_plot(a, pt, 0, 255);
            d_image_blit(a, b, pt);
            d_image_delete(b);
            b = d_image_new(descb);
            d_image_blit(a, b, pt);
            d_image_delete(a);
            d_image_delete(b);
        }
    }

    return 0;
}

/* EOF test-image.c */
