/* 
 * types.h
 * Created: Sat Jan  8 23:25:47 2000 by tek@wiw.org
 * Revised: Thu Apr 19 04:31:17 2001 by tek@wiw.org
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
#if defined(THIRTYTWOBIT)
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;
typedef signed   short sword;
typedef signed   long  sdword;
#elif defined(SIXTYFOURBIT)
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned int   dword;
typedef signed   short sword;
typedef signed   int   sdword;
#else
#error No word size defined!
#endif

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

/* Shared audio types */
typedef enum {PCM=0} d_audioencoding_t;

typedef struct d_audiomode_s {
    word frequency;
    byte bitspersample;
    byte nchannels;
    d_audioencoding_t encoding;
} d_audiomode_t;

/* Geometric primitives */
typedef struct d_point_s {
    int x, y;
} d_point_t;

typedef struct d_rect_s {
    d_point_t offset;
    word w, h;
} d_rect_t;

#endif

/* EOF types.h */
