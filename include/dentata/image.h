/* 
 * image.h
 * Created: Tue Jan  9 05:11:42 2001 by tek@wiw.org
 * Revised: Fri Apr 13 16:17:04 2001 by tek@wiw.org
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
extern void d_image_delete(d_image_t *);

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
 * d_image_plot(d_image_t *image, d_point_t point, d_color_t color,
 *              byte alpha)
 * Plots a single pixel on an image. If alpha is enabled, the pixel
 * is set to the supplied alpha value. The alpha parameter should be
 * in the range 0 (transparent) to 255 (opaque) and will be converted
 * appropriately.
 *
 * Takes: image - a pointer to the image on which to draw.
 *        point - the position on the image to draw the pixel.
 *        color - the color to set the pixel.
 *        alpha - the alpha to set the pixel.
 */
extern void d_image_plot(d_image_t *, d_point_t, d_color_t, byte);

/**
 * d_image_wipe(d_image_t *image, d_color_t color, byte alpha)
 * Fills an image with a single solid color. If alpha is enabled,
 * the entire image is set to alpha, which ranges from 0 (transparent)
 * to 255 (opaque).
 *
 * Takes: image - a pointer to the image to wipe.
 *        color - the color to use.
 *        alpha - the alpha value to use.
 */
extern void d_image_wipe(d_image_t *, d_color_t, byte);

/**
 * d_image_silhouette(d_image_t *image, d_color_t color, byte alpha)
 * Silhouttes an image with color. Any pixel in image whose alpha is
 * not 0 will be set to the supplied color and alpha.
 *
 * Takes: image - a pointer to the image to silhouette.
 *        color - the color to use.
 *        alpha - the alpha value to use.
 */
extern void d_image_silhouette(d_image_t *, d_color_t, byte);

/**
 * d_image_extendalpha(d_image_t *image, byte newdepth)
 * Adds or changes the depth of an image's alpha channel.
 * The alpha is scaled appropriately, and set opaque if the
 * channel has just been added. If newdepth is less than
 * the current alpha depth, the alpha channel is lossily reduced.
 *
 * Takes: image - a pointer to the image to extend.
 *        newdepth - the new alpha depth.
 *
 * Returns: failure if something went wrong or the alpha depth was
 *          invalid, success otherwise.
 */
extern bool d_image_extendalpha(d_image_t *, byte);

/**
 * d_image_convertdepth(d_image_t *image, byte newdepth)
 * Adds or changes the depth of an image.
 *
 * Takes: image - a pointer to the image to extend.
 *        newdepth - the new bpp.
 *
 * Returns: failure if something went wrong or the new depth was
 *          invalid, success otherwise.
 */
extern bool d_image_convertdepth(d_image_t *, byte);

#endif

/* EOF image.h */
