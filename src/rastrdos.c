/* 
 * rastrdos.c
 * Created: Thu Jul 12 13:21:53 2001 by tek@wiw.org
 * Revised: Fri Jul 13 04:46:43 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/raster.h>

bool d_raster_detectvga(void);
void d_raster_restoretextmode(void);

static enum { VGA = 0, VESA = 1 } chipset;
static d_rasterdescription_t curmode;

bool d_raster_new(void)
{
    bool status;

    status = d_raster_detectvga();
    if(status == success)
        chipset = VGA;
    return status;
}

void d_raster_delete(void)
{
    d_raster_restoretextmode();
    return;
}

bool d_raster_setmode(d_rasterdescription_t mode)
{
    return failure;
}

d_rasterdescription_t *d_raster_getmodes(int *nmodes)
{
    switch(chipset) {
    case VGA:
        *nmodes = nvgamodes;
        return vgamodes;

    case VESA:
        /* FIXME */
/*        return d_raster_getvesamodes(nmodes); */

    default:
        *nmodes = 0;
        return NULL;
    }
}

d_rasterdescription_t d_raster_getcurrentmode(void)
{
    return curmode;
}

byte *d_raster_getgfxptr(void)
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

/* EOF rastrdos.c */
