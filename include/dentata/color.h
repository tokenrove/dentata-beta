/**
 * color.h
 * Created: Wed Jan 24 11:19:41 2001 by tek@wiw.org
 * Revised: Sun Jun 24 10:14:37 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: color
 *
 * Change is expected to occur to this API upon the integration
 * of grayscale routines.
 *
 * Include dentata/types.h and dentata/image.h before dentata/color.h.
 */

#ifndef DENTATA_COLOR_H
#define DENTATA_COLOR_H

/**
 * d_color_fromrgb(d_image_t *p, byte r, byte g, byte b)
 * Convert 24bpp RGB to a format apropriate for a specific image.
 *
 * Takes: p - the image which defines the local format.
 *        r,g,b - the color to pack (red, green, blue components).
 *
 * Returns: a d_color_t in the local format.
 */
extern d_color_t d_color_fromrgb(d_image_t *, byte, byte, byte);

/**
 * d_color_frompackedrgb(d_image_t *p, dword rgb)
 * Convert 24bpp RGB to a format appropriate for a specific image,
 * as d_color_fromrgb.
 *
 * Takes: p - the image which defines the local format.
 *        rgb - the RGB color, packed as (msb)0RGB(lsb).
 *
 * Returns: a d_color_t in the local format.
 */
extern d_color_t d_color_frompackedrgb(d_image_t *, dword);

/**
 * d_color_torgb(d_image_t *p, d_color_t c, byte *r, byte *g, byte *b)
 * Convert a local format color to 24bpp RGB.
 *
 * Takes: p - the image which defines the local format.
 *        c - the color in a local format.
 *        r,g,b - pointers to the bytes where the red, green, and blue
 *                component values should be stored, respectively.
 */
extern void d_color_torgb(d_image_t *, d_color_t, byte *, byte *, byte *);

/**
 * d_color_unpack(byte bpp, d_color_t c, byte *r, byte *g, byte *b)
 * Converts the supplied color from a local format to generic 24-bpp RGB.
 * Note that this isn't always defined, so d_color_torgb is
 * suggested when the bpp isn't guaranteed to be one of 15, 16, or 24.
 *
 * Takes: bpp - the bits per pixel of c.
 *        c - the color to unpack.
 *        r,g,b - pointers to the bytes where the red, green, and blue
 *                component values should be stored, respectively.
 *
 * Returns: the appropriate RGB value, in a d_color_t.
 */
extern void d_color_unpack(byte, d_color_t, byte *, byte *, byte *);

/**
 * d_color_pack(byte bpp, byte r, byte g, byte b)
 * Converts the supplied color from 24-bpp RGB to a local format.
 * Note that this isn't always defined, so d_color_fromrgb is
 * suggested when the bpp isn't guaranteed to be one of 15, 16, or 24.
 *
 * Takes: bpp - the bits per pixel of the local format desired.
 *        r,g,b - the color to pack (red, green, blue components).
 *
 * Returns: the appropriate local value, in a d_color_t.
 */
extern d_color_t d_color_pack(byte, byte, byte, byte);

#endif

/* EOF color.h */
