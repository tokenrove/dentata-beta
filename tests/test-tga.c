/* 
 * test-tga.c
 * Created: Sun Feb 25 04:36:16 2001 by tek@wiw.org
 * Revised: Sun Feb 25 04:39:50 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/tga.h>
#include <dentata/image.h>

int main(void)
{
    d_image_t *p;

    p = d_tga_load("data/tga8.tga");
    if(p == NULL)
        d_error_fatal("test-tga: Test 1 failed!\n");

    d_image_delete(p);
    return 0;
}

/* EOF test-tga.c */
