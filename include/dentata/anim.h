/* 
 * anim.h
 * Created: Mon Jan 29 00:22:31 2001 by tek@wiw.org
 * Revised: Thu Apr 19 04:12:44 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta
 * Animation module
 *
 * Include dentata/image.h before this file.
 */

#ifndef DENTATA_ANIM_H
#define DENTATA_ANIM_H

typedef struct d_anim_s {
    word nframes, curframe;
    d_image_t **frames;
    byte framelag, counter;
} d_anim_t;

extern d_anim_t * d_anim_new(byte);
extern void       d_anim_delete(d_anim_t *);
extern d_anim_t * d_anim_dup(d_anim_t *);

extern void       d_anim_addframe(d_anim_t *, d_image_t *);
extern d_image_t *d_anim_curframe(d_anim_t *);
extern d_image_t *d_anim_nextframe(d_anim_t *);
extern void       d_anim_merge(d_anim_t *, d_anim_t *, word);

#endif

/* EOF anim.h */
