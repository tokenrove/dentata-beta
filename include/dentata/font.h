/* 
 * font.h
 * Created: Thu Jan 25 10:37:21 2001 by tek@wiw.org
 * Revised: Mon Jan 29 12:12:07 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta
 * Font module
 */

#ifndef DENTATA_FONT_H
#define DENTATA_FONT_H

#include <dentata/color.h>
#include <dentata/image.h>
#include <dentata/primitives.h>
#include <dentata/types.h>

#define FONT_NCHARS 256

typdef struct d_font_s {
    byte w, h;
    d_rasterdescription_t desc;
    d_image_t *chars[FONT_NCHARS];
} d_font_t;

extern d_font_t *d_font_loadraw(char *, byte, byte, d_color_t);
extern void      d_font_print(d_image_t *, d_font_t *, d_point_t, byte *, ...);
extern void      d_font_delete(d_font_t *);

#endif

/* EOF font.h */
