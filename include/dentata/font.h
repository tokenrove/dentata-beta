/* 
 * font.h
 * Created: Thu Jan 25 10:37:21 2001 by tek@wiw.org
 * Revised: Fri Apr 13 20:37:57 2001 by tek@wiw.org
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

#define FONT_MAXCHARS 256

typedef struct d_font_s {
    d_rasterdescription_t desc;
    int start, nchars;
    d_image_t **chars;
} d_font_t;

extern d_font_t *d_font_load(char *);
extern void d_font_printf(d_image_t *, d_font_t *, d_point_t, byte *, ...);
extern word d_font_gettextwidth(d_font_t *, byte *, ...);
extern void d_font_delete(d_font_t *);

#endif

/* EOF font.h */
