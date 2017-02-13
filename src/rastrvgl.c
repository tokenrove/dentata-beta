/* 
 * rastrvgl.c
 * Created: Sat Jan  8 23:52:54 2000 by tek@wiw.org
 * Revised: Sun Jul 15 06:32:36 2001 by tek@wiw.org
 * Copyright 2000 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * SVGAlib raster implementation
 */

#include <dentata/types.h>
#include <dentata/raster.h>
#include <dentata/memory.h>
#include <dentata/error.h>

#include <vga.h>

bool d_raster_new(void);
void d_raster_delete(void);

bool d_raster_setmode(d_rasterdescription_t);
d_rasterdescription_t *d_raster_getmodes(int *);
d_rasterdescription_t d_raster_getcurrentmode(void);

byte *d_raster_getgfxptr(void);
void d_raster_update(void);

/* static variables */
static byte *raster_vbuf;
static d_rasterdescription_t raster_curmode;


bool d_raster_new(void)
{
    int ret;
    
    ret = vga_init();
    if(ret != 0) {
        d_error_push("vga_init failed.");
        return failure;
    }
    raster_vbuf = NULL;
    return success;
}

void d_raster_delete(void)
{
    d_memory_delete(raster_vbuf);
    vga_setmode(TEXT);
    return;
}

bool d_raster_setmode(d_rasterdescription_t mode)
{
    vga_modeinfo *minf;
    int i, ret;

    if(mode.cspace == grayscale) {
        d_error_push("grayscale modes are not supported "
                     "with this driver.");
        return failure;
    }

    for(i = 1; i < vga_lastmodenumber(); i++) {
        if(vga_hasmode(i) == 0) continue;
        
        minf = vga_getmodeinfo(i);
        if(minf->width         == mode.w   &&
           minf->height        == mode.h   &&
           minf->colors        == 1<<mode.bpp) {
            
            ret = vga_setmode(i);
            if(ret != 0) {
                d_error_push("vga_setmode failed.");
                return failure;
            }

            raster_curmode = mode;
            
            if(raster_vbuf != NULL)
                d_memory_delete(raster_vbuf);

            raster_vbuf = d_memory_new((mode.w*mode.h*mode.bpp+7)/8);
            if(raster_vbuf == NULL) {
                d_error_push("memory allocation failed for "
                             "raster_vbuf.");
                return failure;
            }

            return success;
        }
    }
    d_error_push("no such mode available.");
    return failure;
}

static int log2(int x)
{
    int i;

    for(i = 0; x > 1; i++) x >>= 1;
    return i;
}

d_rasterdescription_t *d_raster_getmodes(int *nmodes)
{
    d_rasterdescription_t *modes;
    vga_modeinfo *minf;
    int i;

    modes = NULL;
    *nmodes = 0;

    for(i = 1; i < vga_lastmodenumber(); i++) {
        if(vga_hasmode(i) == 0) continue;

        (*nmodes)++;
        modes = d_memory_resize(modes, *nmodes*sizeof(d_rasterdescription_t));
        if(modes == NULL) {
            d_error_push("d_raster_getmodes: d_memory_resize failed.");
            return NULL;
        }
        
        minf = vga_getmodeinfo(i);
        modes[*nmodes-1].w = minf->width;
        modes[*nmodes-1].h = minf->height;
        modes[*nmodes-1].bpp = log2(minf->colors);
        modes[*nmodes-1].alpha = 0;
        if(modes[*nmodes-1].bpp == 8)
            modes[*nmodes-1].paletted = true;
        else
            modes[*nmodes-1].paletted = false;
        modes[*nmodes-1].cspace = RGB;
    }
    return modes;
}


d_rasterdescription_t d_raster_getcurrentmode(void)
{
    return raster_curmode;
}

byte *d_raster_getgfxptr(void)
{
    return raster_vbuf;
}

void d_raster_update(void)
{
    int i, inc;
    register byte *vbuf;

    inc = (raster_curmode.w*raster_curmode.bpp+7)/8;
    vbuf = raster_vbuf;

    for(i = 0; i < raster_curmode.h; i++, vbuf += inc)
        vga_drawscanline(i, vbuf);

    return;
}

void d_raster_setpalette(d_palette_t *p)
{
    int i, pal[D_NCLUTITEMS*D_BYTESPERCOLOR];

    for(i = 0; i < D_NCLUTITEMS*D_BYTESPERCOLOR; i++) {
        pal[i] = p->clut[i]>>2;
    }
    vga_setpalvec(0, D_NCLUTITEMS, pal);
    return;
}

void d_raster_getpalette(d_palette_t *p)
{
    int i, pal[D_NCLUTITEMS*D_BYTESPERCOLOR];

    vga_getpalvec(0, D_NCLUTITEMS, pal);
    for(i = 0; i < D_NCLUTITEMS*D_BYTESPERCOLOR; i++)
        p->clut[i] = pal[i]<<2;
    return;
}

/* EOF rastrvgl.c */
