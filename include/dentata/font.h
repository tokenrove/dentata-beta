/**
 * font.h
 * Created: Thu Jan 25 10:37:21 2001 by tek@wiw.org
 * Revised: Sun Jun 24 00:21:56 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: font
 *
 * Include dentata/types.h, dentata/color.h, and dentata/image.h
 * before dentata/font.h.
 */

#ifndef DENTATA_FONT_H
#define DENTATA_FONT_H

/* FIXME: Not valid for kanji or other 2-byte fonts */
#define FONT_MAXCHARS 256

typedef struct d_font_s {
    d_rasterdescription_t desc;
    int start, nchars;
    d_image_t **chars;
} d_font_t;

/**
 * d_font_load(char *filename)
 * Loads a font by name. Note that it is good practice to convert the
 * depth and alpha of the font to suit the raster device. See
 * d_font_convertdepth() and d_font_convertalpha().
 *
 * Takes: filename - the name of the font file.
 *
 * Returns: a pointer to the loaded font, or NULL if an error occurred.
 */
extern d_font_t *d_font_load(char *);

/**
 * d_font_delete(d_font_t *fnt)
 * Frees the memory allocated for a font by d_font_load() or d_font_dup().
 *
 * Takes: fnt - the font to deallocate.
 */
extern void d_font_delete(d_font_t *);

/**
 * d_font_dup(d_font *fnt)
 * Makes a deep copy of the font.
 *
 * Takes: fnt - the font to copy.
 *
 * Returns: the newly copied font, or NULL if something went wrong.
 */
extern d_font_t *d_font_dup(d_font_t *);

/**
 * d_font_printf(d_image_t *img, d_font_t *fnt, d_point_t pt, byte *fmt, ...)
 * Prints a formatted message to the supplied image using the supplied font,
 * as printf(3). Note that a more limited number of formatting options is
 * available, however.
 *
 * Takes: img - the image to affect.
 *        fnt - the font to use.
 *        pt - the offset into the image at which to start placing characters.
 *             (upper-left corner)
 *        fmt - the formatting string, as in printf(3).
 *        ... - the additional arguments, as in printf(3).
 */
extern void d_font_printf(d_image_t *, d_font_t *, d_point_t, byte *, ...);

/**
 * d_font_gettextwidth(d_font_t *fnt, byte *fmt, ...)
 * Determines the number of pixels of horizontal resolution required to
 * display a formatted string using the specified font.
 *
 * Takes: fnt - the font to use.
 *        fmt - the formatting string, as in printf(3).
 *        ... - the additional arguments, as in printf(3).
 *
 * Returns: the width of the formatted string in pixels.
 */
extern word d_font_gettextwidth(d_font_t *, byte *, ...);

/**
 * d_font_convertdepth(d_font_t *fnt, byte newbpp)
 * Converts the color depth of each character in the font, as per
 * d_image_convertdepth(). See d_image_convertdepth()'s documentation
 * for some caveats.
 *
 * Takes: fnt - the font to convert.
 *        newbpp - the new depth to use.
 *
 * Returns: failure if something went wrong or the new depth was
 *          invalid, success otherwise.
 */
extern bool d_font_convertdepth(d_font_t *, byte);

/**
 * d_font_convertalpha(d_font_t *fnt, byte newalpha)
 * Converts the alpha (translucency) depth of each character in the font,
 * as per d_image_convertalpha(). See d_image_convertalpha()'s documentation
 * for some caveats.
 *
 * Takes: fnt - the font to convert.
 *        newalpha - the new alpha depth to use.
 *
 * Returns: failure if something went wrong or the alpha depth was
 *          invalid, success otherwise.
 */
extern bool d_font_convertalpha(d_font_t *, byte);

/**
 * d_font_silhouette(d_font_t *fnt, d_color_t c, byte alpha)
 * Silhouttes each character in the font with the supplied color and
 * alpha. Any pixel in image whose alpha is 0 will not be set to the
 * supplied color and alpha.
 *
 * Takes: fnt - the font to affect.
 *        color - the color to use for the silhouette.
 *        alpha - the alpha value to use for the silhouette.
 */
extern void d_font_silhouette(d_font_t *, d_color_t, byte);

#endif

/* EOF font.h */
