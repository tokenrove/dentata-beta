/* 
 * image.h
 * Created: Tue Jan  9 05:11:42 2001 by tek@wiw.org
 * Revised: Thu Jan 25 11:03:03 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * Image module
 */

#ifndef DENTATA_IMAGE_H
#define DENTATA_IMAGE_H

#include <dentata/primitives.h>
#include <dentata/types.h>

typedef struct d_image_s {
    byte *data, *alpha;
    d_rasterdescription_t desc;
    d_palette_t palette;
} d_image_t;

#include <dentata/color.h>

extern d_image_t *d_image_new(d_rasterdescription_t);
extern void       d_image_delete(d_image_t *);
extern d_image_t *d_image_dup(d_image_t *);

extern void       d_image_plot(d_image_t *, d_point_t, d_color_t);
extern void       d_image_wipe(d_image_t *, d_color_t);

#endif

/* EOF image.h */
