/* 
 * raster_null.c
 * Created: Sat Feb 24 19:54:17 2001 by tek@wiw.org
 * Revised: Sun Apr  8 00:21:27 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Null raster module for testing, platforms without support, et cetera.
 */

#include <dentata/raster.h>

bool d_raster_new(void)
{
    return success;
}

void d_raster_delete(void)
{
    return;
}

bool d_raster_setmode(d_rasterdescription_t mode)
{
    return true;
}

d_rasterdescription_t *d_raster_getmodes(int *nmodes)
{
    *nmodes = 0;
    return NULL;
}

d_rasterdescription_t d_raster_getcurrentmode(void)
{
    d_rasterdescription_t curmode;

    curmode.w = curmode.h = 0;
    curmode.bpp = 0;
    curmode.cspace = grayscale;
    curmode.paletted = false;

    return curmode;
}

byte *d_raster_getgfxpointer(void)
{
    return NULL;
}

void d_raster_update(void)
{
    return;
}

void d_raster_setpalette(d_palette_t *palette)
{
    return;
}

void d_raster_getpalette(d_palette_t *palette)
{
    return;
}

/* EOF raster_null.c */
