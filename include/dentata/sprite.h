/**
 * sprite.h
 * Created: Thu Jan 25 11:03:32 2001 by tek@wiw.org
 * Revised: Sat Jun 23 03:28:45 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: sprite
 *
 * Include dentata/anim.h and dentata/types.h before this file.
 */

#ifndef DENTATA_SPRITE_H
#define DENTATA_SPRITE_H

typedef struct d_sprite_s {
    word nanims, curanim;
    d_anim_t **anim;
    bool animated;
    d_point_t pos;
    d_rect_t clip;
} d_sprite_t;

extern d_sprite_t *d_sprite_new(void);
extern d_sprite_t *d_sprite_dup(d_sprite_t *);
extern void        d_sprite_delete(d_sprite_t *);
extern d_sprite_t *d_sprite_loadfromspr(char *);

extern void        d_sprite_addanim(d_sprite_t *, d_anim_t *);
extern word        d_sprite_curwidth(d_sprite_t *);
extern word        d_sprite_curheight(d_sprite_t *);

#endif

/* EOF sprite.h */
