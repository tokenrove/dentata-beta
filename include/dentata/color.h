/* 
 * color.h
 * Created: Wed Jan 24 11:19:41 2001 by tek@wiw.org
 * Revised: Wed Jan 31 15:12:54 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta
 * Colorspace module
 */

#ifndef DENTATA_COLOR_H
#define DENTATA_COLOR_H

#include <dentata/types.h>

typedef dword d_color_t;

#include <dentata/image.h>

extern d_color_t d_color_fromrgb(d_image_t *, byte, byte, byte);
extern d_color_t d_color_frompackedrgb(d_image_t *, dword);

#endif

/* EOF color.h */
