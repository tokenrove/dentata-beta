/* 
 * types.h
 * Created: Sat Jan  8 23:25:47 2000 by tek@wiw.org
 * Revised: Sun Feb 25 05:04:35 2001 by tek@wiw.org
 * Copyright 2000 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * Basic/shared types
 */

#ifndef DENTATA_TYPES_H
#define DENTATA_TYPES_H

/* Basic types */
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

typedef enum { false=0, failure=false,
               true=1,  success=true } bool;

#ifndef NULL
#define NULL ((void *)0)
#endif

/* Shared graphics types */
typedef enum {RGB=0, grayscale} d_colorspace_t;

typedef struct d_rasterdescription_s {
    word w, h;
    byte bpp, alpha;
    bool paletted;
    d_colorspace_t cspace;
} d_rasterdescription_t;

#define D_NCLUTITEMS    256
#define D_BYTESPERCOLOR 3

typedef dword d_color_t;

typedef struct d_palette_s {
    byte alphaent;
    byte clut[D_NCLUTITEMS*D_BYTESPERCOLOR];
} d_palette_t;

typedef enum { none = 0, pixel = 1, rectangle = 2 } d_clipstrategy_t;

/* Shared audio types */
typedef enum {PCM=0} d_audioencoding_t;

typedef struct d_audiomode_s {
    word frequency;
    byte bitspersample;
    byte nchannels;
    d_audioencoding_t encoding;
} d_audiomode_t;

#endif

/* EOF types.h */
