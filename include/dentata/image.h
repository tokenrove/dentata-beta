/**
 * image.h
 * Created: Tue Jan  9 05:11:42 2001 by tek@wiw.org
 * Revised: Sun Jun 24 00:59:38 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: image
 *
 * Before thinking of monkeying with the d_image_t members
 * directly, remember that they may be implemented in very strange
 * ways on some systems, and so it is best to avoid touching
 * them except through the standard d_image_* functions. For many
 * platforms, these functions are implemented very efficiently.
 *
 * Also, some APIs here are likely to change. These changes are
 * generally mentioned in the documentation for the individual
 * function, but some other notable changes to come include addition
 * of functions providing scaling and cropping, among other
 * things.
 *
 * Include dentata/types.h before this file.
 */

#ifndef DENTATA_IMAGE_H
#define DENTATA_IMAGE_H

typedef struct d_image_s {
    /* private */
    byte *data, *alpha;
    bool imageownsdata;
    /* public */
    d_rasterdescription_t desc;
    d_palette_t palette;
} d_image_t;

typedef enum { bounding = 0, pixel = 1, alphathresh = 2 } d_collidemode_t;

/**
 * d_image_new(d_rasterdescription_t desc)
 * Creates a new image appropriate to the description provided.
 *
 * Takes: desc - the parameters by which to create the image.
 *
 * Returns: a pointer to the newly created image.
 */
extern d_image_t *d_image_new(d_rasterdescription_t);

/**
 * d_image_delete(d_image_t *p)
 * Deletes an image and any space used by its underlying components.
 *
 * Takes: p - a pointer to the image to delete.
 */
extern void d_image_delete(d_image_t *);

/**
 * d_image_dup(d_image_t *p)
 * Makes a deep copy of an image.
 *
 * Takes: p - a pointer to the image to duplicate.
 *
 * Returns: a pointer to the copy of p.
 */
extern d_image_t *d_image_dup(d_image_t *);

/**
 * d_image_setdataptr(d_image_t *p, byte *ptr, bool imageownsdata)
 * Changes the data pointer for this image to the one specified by ptr.
 * This frees the space used by the previous data pointer, assuming
 * it was handled by the image module. (if the data pointer had previously
 * been set by setdataptr with imageownsdata set to false, the data
 * will not be freed)
 *
 * Takes: p - a pointer to the image to change.
 *        ptr - the new data pointer.
 *        imageownsdata - a bool specifying whether to handle the
 *                        new data pointer on delete or not.
 *
 * Returns: failure if anything went wrong, success otherwise.
 */
extern bool d_image_setdataptr(d_image_t *, byte *, bool);

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
 * d_image_getpelalpha(d_image_t *p, d_point_t pt)
 * Gets the alpha of a given pixel in an image.
 *
 * Takes: p - the image to inspect.
 *        pt - the offset into the image of the pixel.
 *
 * Returns: the alpha value of this pixel.
 */
extern byte d_image_getpelalpha(d_image_t *, d_point_t);

/**
 * d_image_getpelcolor(d_image_t *p, d_point_t pt)
 * Gets the color of a given pixel in an image.
 *
 * Takes: p - the image to inspect.
 *        pt - the offset into the image of the pixel.
 *
 * Returns: the color of this pixel in local format.
 */
extern d_color_t d_image_getpelcolor(d_image_t *, d_point_t);

/**
 * d_image_setpelalpha(d_image_t *p, d_point_t pt, byte alpha)
 * Sets the alpha of a given pixel in an image.
 *
 * Takes: p - the image to affect.
 *        pt - the offset into the image of the pixel.
 *        alpha - the new alpha value.
 */
extern void d_image_setpelalpha(d_image_t *, d_point_t, byte);

/**
 * d_image_setpelcolor(d_image_t *p, d_point_t pt, d_color_t c)
 * Sets the color of a given pixel in an image.
 *
 * Takes: p - the image to affect.
 *        pt - the offset into the image of the pixel.
 *        c - the new color value, in local format.
 */
extern void d_image_setpelcolor(d_image_t *, d_point_t, d_color_t);

/**
 * d_image_wipe(d_image_t *image, d_color_t color, byte alpha)
 * Fills an image with a single solid color. If alpha is enabled,
 * the entire image is set to the supplied alpha.
 *
 * Takes: image - a pointer to the image to wipe.
 *        color - the color to use.
 *        alpha - the alpha value to use.
 */
extern void d_image_wipe(d_image_t *, d_color_t, byte);

/**
 * d_image_silhouette(d_image_t *image, d_color_t color, byte alpha)
 * Silhouttes an image with the supplied color and alpha. Any pixel in
 * the image whose alpha is 0 will not be set to the supplied color and
 * alpha.
 *
 * Takes: image - a pointer to the image to silhouette.
 *        color - the color to use.
 *        alpha - the alpha value to use.
 */
extern void d_image_silhouette(d_image_t *, d_color_t, byte);

/**
 * d_image_convertalpha(d_image_t *image, byte newdepth)
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
extern bool d_image_convertalpha(d_image_t *, byte);

/**
 * d_image_convertdepth(d_image_t *image, byte newbpp)
 * Adds or changes the color depth of an image.
 *
 * Takes: image - a pointer to the image to extend.
 *        newbpp - the new color depth.
 *
 * Returns: failure if something went wrong or the new depth was
 *          invalid, success otherwise.
 */
extern bool d_image_convertdepth(d_image_t *, byte);

/**
 * d_image_blit(d_image_t *dst, d_image_t *src, d_point_t pt)
 * Copies a rectangular chunk of the source image onto the destination
 * image, doing alpha blending if appropriate. Clipping of the source
 * to fit within the destination is done internally. Note that this
 * function does _not_ do alpha composition -- that is, the
 * destination image's alpha bits are unmodified. See d_image_alphacomp()
 * for that functionality.
 *
 * Takes: dst - the destination image.
 *        src - the source image.
 *        pt - the offset of src from dst.
 */
extern void d_image_blit(d_image_t *, d_image_t *, d_point_t);

/**
 * d_image_alphacomp(d_image_t *dst, d_image_t *src, d_point_t pt)
 * Sum a rectangular chunk of the alphas of two images. The alpha
 * depth must be the same in both. This function typically goes
 * hand in hand with d_image_blit(), and functions similarly.
 *
 * Takes: dst - the destination image.
 *        src - the source image.
 *        pt - the offset of src from dst.
 */
extern void d_image_alphacomp(d_image_t *, d_image_t *, d_point_t);

/**
 * d_image_collide(d_image_t *a, d_image_t *b, d_point_t pt,
 *                 d_collidemode_t cmode)
 * Checks if a and b collide, based on the collide mode. The only
 * supported collidemodes at the moment are `bounding', which reports
 * a collision if a and b are within each-other's clip rectangles,
 * `pixel', which reports a collision if a pixel of both a and b is
 * opaque, and `alphathresh', which reports a collision if the sum of
 * the alpha of a pixel from a and the corresponding pixel from b is
 * greater than or equal to 255.
 *
 * Takes: a, b - the images to check for collision.
 *        pt - the offset of b from a.
 *        cmode - the collide mode, one of bounding, pixel, or
 *                alphathresh.
 *
 * Returns: true on collision, false otherwise.
 */
extern bool d_image_collide(d_image_t *, d_image_t *, d_point_t,
                            d_collidemode_t);

/**
 * d_image_rotate(d_image_t *p, byte theta)
 * Rotate an image by a given angle, producing a new image.
 * This routine is a placeholder for a forthcoming more flexible
 * routine which will allow rotation in situ as well as the creation
 * of a new image.
 *
 * Takes: p - the image to rotate.
 *        theta - the angle by which to rotate the image. It is in
 *                the range of 0 to 255, with 0 being equivalent to
 *                0 degrees, 128 being 180 degrees, 64 being 90 degrees,
 *                et cetera.
 *
 * Returns: the newly rotated image. Note that this is a seperate image
 *          from p.
 */
extern d_image_t *d_image_rotate(d_image_t *, byte);

#endif

/* EOF image.h */
