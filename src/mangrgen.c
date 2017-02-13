/* 
 * mangrgen.c
 * Created: Sun Jul 15 01:15:36 2001 by tek@wiw.org
 * Revised: Fri Jul 27 04:07:04 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>
#include <dentata/tilemap.h>
#include <dentata/sprite.h>
#include <dentata/manager.h>
#include <dentata/memory.h>
#include <dentata/error.h>
#include <dentata/set.h>
#include <dentata/random.h>
#include <dentata/util.h>

bool d_manager_new(void);
void d_manager_delete(void);
word d_manager_getmaxlayers(void);
word d_manager_getmaxsprites(void);
bool d_manager_addsprite(d_sprite_t *, word *, char);
bool d_manager_removesprite(word);
bool d_manager_addimagelayer(d_image_t *, word *, char);
bool d_manager_addtilemaplayer(d_tilemap_t *, word *, char);
void d_manager_wipelayers(void);
void d_manager_wipesprites(void);
void d_manager_draw(d_image_t *);
void d_manager_pansprite(word, int, int);
void d_manager_jumpsprite(word handle, word x, word y);
void d_manager_setscrollparameters(bool, char);
void d_manager_pan(int, int);
void d_manager_jump(int, int);
d_rect_t d_manager_getvirtualrect(void);

static void drawlayers(d_image_t *dst);
static void drawsprites(d_image_t *dst);
static void drawtilemap(d_image_t *dst, d_tilemap_t *tm, d_point_t pt);
static void drawimagelayer(d_image_t *dst, d_image_t *im, d_point_t pt, char depth);

#define KEYMAX 32767

typedef struct layer_s {
    enum { image = 0, tilemap = 1 } type;
    union {
        d_image_t *image;
        d_tilemap_t *tilemap;
    } data;
    bool loopx, loopy;
    d_point_t offs;
    char priority;
} layer_t;

typedef struct sprite_s {
    d_sprite_t *data;
    d_point_t offs;
    char priority;
} sprite_t;

static d_set_t *layerset;
static d_set_t *spriteset;
static bool parallax;
static char midground;
static d_point_t camera;


bool d_manager_new(void)
{
    layerset = d_set_new(255);
    spriteset = d_set_new(255);
    parallax = false;
    midground = 0;
    camera.x = 0;
    camera.y = 0;
    return success;
}


void d_manager_delete(void)
{
    dword key;
    void *p;
    d_iterator_t it;

    d_iterator_reset(&it);
    while(key = d_set_nextkey(&it, layerset), key != D_SET_INVALIDKEY) {
        d_set_fetch(layerset, key, &p);
        d_memory_delete(p);
    }
    d_set_delete(layerset);
    layerset = NULL;

    d_iterator_reset(&it);
    while(key = d_set_nextkey(&it, spriteset), key != D_SET_INVALIDKEY) {
        d_set_fetch(spriteset, key, &p);
        d_memory_delete(p);
    }
    d_set_delete(spriteset);
    spriteset = NULL;
    return;
}


word d_manager_getmaxlayers(void)
{
    return KEYMAX;
}


word d_manager_getmaxsprites(void)
{
    return KEYMAX;
}


bool d_manager_addsprite(d_sprite_t *spr, word *handle, char priority)
{
    dword key;
    sprite_t *p;
    bool status;

    key = d_set_getunusedkey(spriteset);
    if(key > KEYMAX) {
        d_error_push("invalid key.");
        return failure;
    }

    p = d_memory_new(sizeof(sprite_t));
    if(p == NULL) {
        d_error_push("failed to allocate internal sprite "
                     "structure.");
        return failure;
    }

    p->priority = priority;
    p->data = spr;
    p->offs.x = p->offs.y = 0;

    status = d_set_add(spriteset, key, p);
    if(status == failure) {
        d_error_push("failed to add sprite to internal set.");
        return failure;
    }

    if(handle != NULL)
        *handle = key;
    return success;
}

bool d_manager_removesprite(word key)
{
    return d_set_remove(spriteset, key);
}


bool d_manager_addimagelayer(d_image_t *im, word *handle, char priority)
{
    dword key;
    layer_t *p;
    bool status;

    key = d_set_getunusedkey(layerset);
    if(key > KEYMAX)
        return failure;

    p = d_memory_new(sizeof(layer_t));
    if(p == NULL)
        return failure;
    p->type = image;
    p->priority = priority;
    p->data.image = im;
    p->offs.x = p->offs.y = 0;

    status = d_set_add(layerset, key, p);
    if(status == failure)
        return failure;

    if(handle != NULL)
        *handle = key;
    return success;
}


bool d_manager_addtilemaplayer(d_tilemap_t *tm, word *handle, char priority)
{
    dword key;
    layer_t *p;
    bool status;

    key = d_set_getunusedkey(layerset);
    if(key > KEYMAX)
        return failure;

    p = d_memory_new(sizeof(layer_t));
    if(p == NULL)
        return failure;
    p->type = tilemap;
    p->priority = priority;
    p->data.tilemap = tm;
    p->offs.x = p->offs.y = 0;

    status = d_set_add(layerset, key, p);
    if(status == failure)
        return failure;

    if(handle != NULL)
        *handle = key;
    return success;
}


void d_manager_wipelayers(void)
{
    dword key;
    layer_t *p;
    d_iterator_t it;

    d_iterator_reset(&it);
    while(key = d_set_nextkey(&it, layerset), key != D_SET_INVALIDKEY) {
        d_set_fetch(layerset, key, (void **)&p);
        d_memory_delete(p);
        d_set_remove(layerset, key);
        d_iterator_reset(&it);
    }
    return;
}


void d_manager_wipesprites(void)
{
    dword key;
    sprite_t *p;
    d_iterator_t it;

    d_iterator_reset(&it);
    while(key = d_set_nextkey(&it, spriteset), key != D_SET_INVALIDKEY) {
        d_set_fetch(spriteset, key, (void **)&p);
        d_memory_delete(p);
        d_set_remove(spriteset, key);
        d_iterator_reset(&it);
    }
    return;
}


void d_manager_draw(d_image_t *dst)
{
    drawlayers(dst);
    drawsprites(dst);
    return;
}


void drawlayers(d_image_t *dst)
{
    dword key;
    layer_t *p;
    d_iterator_t it;

    d_iterator_reset(&it);
    while(key = d_set_nextkey(&it, layerset), key != D_SET_INVALIDKEY) {
        if(key > KEYMAX) {
            d_error_debug("odd key %d\n", key);
            continue;
        }
        d_set_fetch(layerset, key, (void **)&p);
        if(p->type == image) {
            drawimagelayer(dst, p->data.image, p->offs, p->priority*2);
        } else
            drawtilemap(dst, p->data.tilemap, p->offs);
    }
    return;
}


int roundiv(int a, int m)
{
    if(a < 0) a -= m-1;
    a /= m;
    return a;
}


int xmod(int a, int m)
{
    a %= m;
    if(a < 0) a += m;
    return a;
}


void drawtilemap(d_image_t *dst, d_tilemap_t *tm, d_point_t pt)
{
    int i, j, tile;
    d_point_t pt2;

    for(i = roundiv(camera.y, tm->tiledesc.h);
        i <= (camera.y+dst->desc.h)/tm->tiledesc.h;
        i++) {
        for(j = roundiv(camera.x, tm->tiledesc.w);
            j <= (camera.x+dst->desc.w)/tm->tiledesc.w;
            j++) {
            tile = tm->map[xmod(i, tm->h)*tm->w+xmod(j, tm->w)];
            pt2.x = j*tm->tiledesc.w-camera.x;
            pt2.y = i*tm->tiledesc.h-camera.y;
            d_image_blit(dst, tm->tiles[tile], pt2);
        }
    }
            
}


void drawimagelayer(d_image_t *dst, d_image_t *im, d_point_t pt, char depth)
{
    int i, j;
    d_point_t pt2;

    for(i = roundiv((camera.y/-depth), im->desc.h)-1;
        i <= ((camera.y/-depth)+dst->desc.h)/im->desc.h;
        i++) {
        for(j = roundiv((camera.x/-depth), im->desc.w)-1;
            j <= ((camera.x/-depth)+dst->desc.w)/im->desc.w;
            j++) {
            if(depth > 0) {
                pt2.x = (j*im->desc.w-camera.x)*depth;
                pt2.y = (i*im->desc.h-camera.y)*depth;
            } else if(depth < 0) {
                pt2.x = j*im->desc.w-(camera.x/(-depth));
                pt2.y = i*im->desc.h-(camera.y/(-depth));
            } else {
                pt2.x = j*im->desc.w-camera.x;
                pt2.y = i*im->desc.h-camera.y;
            }
            d_image_blit(dst, im, pt2);
        }
    }
    return;
}


void drawsprites(d_image_t *dst)
{
    dword key;
    sprite_t *p;
    d_image_t *q;
    d_point_t pt;
    d_iterator_t it;

    d_iterator_reset(&it);
    while(key = d_set_nextkey(&it, spriteset), key != D_SET_INVALIDKEY) {
        if(key > KEYMAX) {
            d_error_debug("odd key %d\n", key);
            continue;
        }
        d_set_fetch(spriteset, key, (void **)&p);
        q = d_sprite_getcurframe(p->data);
        pt = p->offs;
        pt.x -= camera.x;
        pt.y -= camera.y;
        d_image_blit(dst, q, pt);
        d_sprite_stepframe(p->data);
    }
    return;
}


void d_manager_pansprite(word handle, int x, int y)
{
    sprite_t *p;

    d_set_fetch(spriteset, handle, (void **)&p);
    p->offs.x += x;
    p->offs.y += y;
    return;
}


void d_manager_jumpsprite(word handle, word x, word y)
{
    sprite_t *p;

    d_set_fetch(spriteset, handle, (void **)&p);
    p->offs.x = x;
    p->offs.y = y;
    return;
}


void d_manager_setscrollparameters(bool parallax_, char midground_)
{
    parallax = parallax_;
    midground = midground_;
    return;
}


void d_manager_pan(int x, int y)
{
    camera.x += x;
    camera.y += y;
    return;
}


void d_manager_jump(int x, int y)
{
    camera.x = x;
    camera.y = y;
    return;
}


d_rect_t d_manager_getvirtualrect(void)
{
    d_rect_t r;
    dword key;
    layer_t *p;
    d_iterator_t it;

    r.w = r.h = KEYMAX;
    d_iterator_reset(&it);
    while(key = d_set_nextkey(&it, layerset), key != D_SET_INVALIDKEY) {
        if(key > KEYMAX) {
            d_error_debug("odd key %d\n", key);
            continue;
        }
        d_set_fetch(layerset, key, (void **)&p);
        if(p->type == image) {
            /* this is commented out so that images loop forever */
/*            if(p->data.image->desc.w < r.w)
                r.w = p->data.image->desc.w;
            else if(p->data.image->desc.h < r.h)
            r.h = p->data.image->desc.h; */
        } else {
            if(p->data.tilemap->w*p->data.tilemap->tiledesc.w < r.w)
                r.w = p->data.tilemap->w*p->data.tilemap->tiledesc.w;
            if(p->data.tilemap->h*p->data.tilemap->tiledesc.h < r.h)
                r.h = p->data.tilemap->h*p->data.tilemap->tiledesc.h;
        }
    }
    r.offset.x = 0;
    r.offset.y = 0;
    return r;
}

/* EOF mangrgen.c */
