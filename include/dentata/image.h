/* 
 * image.h
 * Created: Tue Jan  9 05:11:42 2001 by tek@wiw.org
 * Revised: Sat May  5 09:24:28 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * Image module
 *
 * Include dentata/types.h before this file.
 */

#ifndef DENTATA_IMAGE_H
#define DENTATA_IMAGE_H

typedef struct d_image_s {
    byte *data, *alpha;
    d_rasterdescription_t desc;
    d_palette_t palette;
} d_image_t;

typedef enum { bounding = 0, pixel = 1, alphathresh = 2 } d_collidemode_t;

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

/**
 * d_image_blit(d_image_t *dst, d_image_t *src, d_point_t pt)
 * Blits src onto dst.
 *
 * Takes: dst - the destination image.
 *        src - the source image.
 *        pt - the offset of src from dst.
 */
extern void d_image_blit(d_image_t *, d_image_t *, d_point_t);

/**
 * d_image_collide(d_image_t *a, d_image_t *b, d_point_t pt,
 *                 d_collidemode_t cmode)
 * Checks if a and b collide, based on the collide mode. The only
 * supported collidemodes at the moment are `pixel', which reports a
 * collision if a pixel of both a and b is opaque, and `alphathresh',
 * which reports a collision if the sum of the alpha of a pixel from a
 * and the corresponding pixel from b is greater than or equal to 255.
 *
 * Takes: a, b - the images to check for collision.
 *        pt - the offset of b from a.
 *        cmode - the collide mode, one of `pixel', or `alphathresh'.
 *
 * Returns: true on collision, false otherwise.
 */
extern bool d_image_collide(d_image_t *, d_image_t *, d_point_t,
                            d_collidemode_t);

extern void d_image_alphacomp(d_image_t *, d_image_t *, d_point_t);
extern d_image_t *d_image_rotate(d_image_t *, byte);
extern byte d_image_getpelalpha(d_image_t *, d_point_t);
extern d_color_t d_image_getpelcolor(d_image_t *, d_point_t);
extern void d_image_setpelalpha(d_image_t *, d_point_t, byte);
extern void d_image_setpelcolor(d_image_t *, d_point_t, d_color_t);
extern void d_image_alphacomp(d_image_t *, d_image_t *, d_point_t);

#endif

/* EOF image.h */
