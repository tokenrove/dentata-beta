/* 
 * tilemap.h
 * Created: Sun Jul 15 00:32:39 2001 by tek@wiw.org
 * Revised: Sun Jul 15 00:52:01 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#ifndef DENTATA_TILEMAP_H
#define DENTATA_TILEMAP_H

#define D_TILEMAP_MAXTILE 255

typedef struct d_tilemap_s {
    d_rasterdescription_t tiledesc;
    word w, h, ntiles;
    byte *map;
    d_image_t **tiles;
} d_tilemap_t;

extern d_tilemap_t *d_tilemap_new(d_rasterdescription_t, word, word);
extern void d_tilemap_delete(d_tilemap_t *);
extern bool d_tilemap_addtileimage(d_tilemap_t *, byte, d_image_t *);
extern byte d_tilemap_gettile(d_tilemap_t *, d_point_t);
extern void d_tilemap_settile(d_tilemap_t *, d_point_t, byte);

#endif

/* EOF tilemap.h */
