/* 
 * image.h
 * Created: Tue Jan  9 05:11:42 2001 by tek@wiw.org
 * Revised: Sun Feb 25 02:35:21 2001 by tek@wiw.org
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

/**
 * d_image_new(d_rasterdescription_t desc)
 * Creates a new d_image_t, with the data and alpha members allocated
 * appropriate to the description provided.
 *
 * Takes: desc - the parameters by which to create the image.
 *
 * Returns: a pointer to the newly created image.
 */
extern d_image_t *d_image_new(d_rasterdescription_t);

/**
 * d_image_delete(d_image_t *p)
 * Deletes a d_image_t and any space used by its underlying components.
 *
 * Takes: p - a pointer to the image to delete.
 */
extern void       d_image_delete(d_image_t *);

/**
 * d_image_dup(d_image_t *p)
 * Makes a deep copy of a d_image_t.
 *
 * Takes: p - a pointer to the image to duplicate.
 *
 * Returns: a pointer to the copy of p.
 */
extern d_image_t *d_image_dup(d_image_t *);

/**
 * d_image_plot(d_image_t *image, d_point_t point, d_color_t color)
 * Plots a single pixel on an image. If alpha is enabled, the pixel
 * is set to be opaque.
 *
 * Takes: image - a pointer to the image on which to draw.
 *        point - the position on the image to draw the pixel.
 *        color - the color to set the pixel.
 */
extern void       d_image_plot(d_image_t *, d_point_t, d_color_t);

/**
 * d_image_wipe(d_image_t *image, d_color_t color)
 * Fills an image with a single solid color. If alpha is enabled,
 * the entire image becomes opaque.
 *
 * Takes: image - a pointer to the image to wipe.
 *        color - the color to use.
 */
extern void       d_image_wipe(d_image_t *, d_color_t);

#endif

/* EOF image.h */
