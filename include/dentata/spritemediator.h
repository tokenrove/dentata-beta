/* 
 * spritemediator.h
 * Created: Wed Jan 24 15:04:46 2001 by tek@wiw.org
 * Revised: Sun Jan 28 15:51:40 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#ifndef DENTATA_SPRITEMEDIATOR_H
#define DENTATA_SPRITEMEDIATOR_H

#include <dentata/image.h>
#include <dentata/primitives.h>
#include <dentata/sprite.h>

extern bool        d_spritemediator_new(void);
extern void        d_spritemediator_delete(void);
extern word        d_spritemediator_add(d_sprite_t *);
extern d_sprite_t *d_spritemediator_fetch(word);
extern void        d_spritemediator_remove(word);
extern void        d_spritemediator_draw(d_image_t *, d_rect_t);

extern void        d_spritemediator_pan(word, int, int);
extern void        d_spritemediator_jump(word, word, word);

extern bool        d_spritemediator_hascollided(word);
extern void        d_spritemediator_resetcollisions(word);


#endif

/* EOF spritemediator.h */
