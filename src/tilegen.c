/* 
 * tilegen.c
 * Created: Sun Jul 15 00:49:46 2001 by tek@wiw.org
 * Revised: Sun Jul 15 14:15:01 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>
#include <dentata/tilemap.h>
#include <dentata/memory.h>
#include <dentata/error.h>

d_tilemap_t *d_tilemap_new(d_rasterdescription_t, word, word);
void d_tilemap_delete(d_tilemap_t *);
bool d_tilemap_addtileimage(d_tilemap_t *, byte, d_image_t *);
byte d_tilemap_gettile(d_tilemap_t *, d_point_t);
void d_tilemap_settile(d_tilemap_t *, d_point_t, byte);


d_tilemap_t *d_tilemap_new(d_rasterdescription_t desc, word w, word h)
{
    d_tilemap_t *p;

    p = d_memory_new(sizeof(d_tilemap_t));
    if(p == NULL)
        return NULL;

    p->tiledesc = desc;
    p->w = w;
    p->h = h;
    p->ntiles = 0;
    p->map = d_memory_new(p->w*p->h);
    if(p->map == NULL)
        return NULL;
    d_memory_set(p->map, 0, p->w*p->h);
    p->tiles = NULL;

    return p;
}


void d_tilemap_delete(d_tilemap_t *p)
{
    int i;

    d_memory_delete(p->map);
    p->map = NULL;
    for(i = 0; i < p->ntiles; i++) {
        if(p->tiles[i] != NULL)
            d_image_delete(p->tiles[i]);
        p->tiles[i] = NULL;
    }
    d_memory_delete(p->tiles);
    p->tiles = NULL;
    d_memory_delete(p);
    return;
}


bool d_tilemap_addtileimage(d_tilemap_t *p, byte tile, d_image_t *image)
{
    int i;

    if(image != NULL)
        if(image->desc.w != p->tiledesc.w ||
           image->desc.h != p->tiledesc.h)
            return failure;

    if(tile >= p->ntiles) {
        p->tiles = d_memory_resize(p->tiles, (tile+1)*sizeof(d_image_t *));
	for(i = p->ntiles; i < tile; i++)
	    p->tiles[i] = NULL;
        p->ntiles = tile+1;
    }
    p->tiles[tile] = image;
    return success;
}


byte d_tilemap_gettile(d_tilemap_t *p, d_point_t pt)
{
    pt.x /= p->tiledesc.w;
    pt.y /= p->tiledesc.h;

    return p->map[pt.x%p->w+(pt.y%p->h)*p->w];
}


void d_tilemap_settile(d_tilemap_t *p, d_point_t pt, byte tile)
{
    p->map[pt.x%p->w+(pt.y%p->h)*p->w] = tile;
    return;
}

/* EOF tilegen.c */
