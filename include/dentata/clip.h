/* 
 * clip.h
 * Created: Mon Jan 29 12:15:17 2001 by tek@wiw.org
 * Revised: Wed Jan 31 15:14:27 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta
 * Clipping module
 */

#ifndef DENTATA_CLIP_H
#define DENTATA_CLIP_H

#include <dentata/primitives.h>
#include <dentata/sprite.h>
#include <dentata/types.h>

extern bool      d_clip_issprclipspr(d_sprite_t *, d_sprite_t *);
extern d_point_t d_clip_wheresprclipspr(d_sprite_t *, d_sprite_t *);

#endif

/* EOF clip.h */
