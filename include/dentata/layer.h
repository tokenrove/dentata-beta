/* 
 * layer.h
 * Created: Sun Jan 28 16:07:12 2001 by tek@wiw.org
 * Revised: Mon Jan 29 13:06:57 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta
 * Layer module
 */

#ifndef DENTATA_LAYER_H
#define DENTATA_LAYER_H

#define D_LAYER_NOLOOP 0
#define D_LAYER_LOOPX  1
#define D_LAYER_LOOPY  2

typdef struct d_layer_s {
    enum { tilemap = 0, image = 1 } type;
    byte loop;
    union {
        d_tilemap_t *tilemap;
        d_image_t *image;
    } contents;
} d_layer_t;

extern d_layer_t *d_layer_new(void);
extern void       d_layer_delete(d_layer_t *);
extern d_layer_t *d_layer_dup(d_layer_t *);

#endif

/* EOF layer.h */
