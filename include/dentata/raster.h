/* 
 * raster.h
 * Created: Sat Jan  8 23:40:41 2000 by tek@wiw.org
 * Revised: Wed Jan 24 11:17:16 2001 by tek@wiw.org
 * Copyright 2000 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * Raster module
 */

#include <dentata/types.h>

#ifndef DENTATA_RASTER_H
#define DENTATA_RASTER_H

extern bool                   d_raster_new(void);
extern void                   d_raster_delete(void);

extern bool                   d_raster_setmode(d_rasterdescription_t);
extern d_rasterdescription_t *d_raster_getmodes(int *);
extern d_rasterdescription_t  d_raster_getcurrentmode(void);

extern byte *                 d_raster_getgfxpointer(void);
extern void                   d_raster_update(void);

extern void                   d_raster_setpalette(d_palette_t *);
extern d_palette_t *          d_raster_getpalette(void);

#endif

/* EOF raster.h */
