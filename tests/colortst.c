/* 
 * colortst.c
 * Created: Tue Jun 26 20:43:44 2001 by tek@wiw.org
 * Revised: Tue Jun 26 20:56:11 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>
#include <dentata/color.h>
#include <dentata/random.h>
#include <dentata/error.h>

#define NRANDITERS 1000

bool test1(d_random_t *rh, byte bpp);

int main(void)
{
    d_random_t *rh;
    int i;
    byte bpp[] = { 15, 16, 24, 0 };

    rh = d_random_new();

    /* test one: consistency between pack and unpack */
    for(i = 0; bpp[i] != 0; i++)
        if(test1(rh, bpp[i]) != success)
            d_error_fatal("Test 1 failed on bpp %d.\n", bpp[i]);

    d_random_delete(rh);
    return 0;
}

bool test1(d_random_t *rh, byte bpp)
{
    byte r, g, b, rp, gp, bp;
    int i;
    d_color_t c, cprime;
    
    for(i = 0; i < NRANDITERS; i++) {
        r = d_random_gen(rh, 256);
        b = d_random_gen(rh, 256);
        g = d_random_gen(rh, 256);
        c = d_color_pack(bpp, r, g, b);
        d_color_unpack(bpp, c, &rp, &gp, &bp);
        cprime = d_color_pack(bpp, rp, gp, bp);
        if(c != cprime) {
            d_error_push("Test 1: pack->unpack->pack inconsistent.");
            return failure;
        }
    }
    return success;
}

/* EOF colortst.c */
