/* 
 * sprite.h
 * Created: Sat Jul 14 02:52:27 2001 by tek@wiw.org
 * Revised: Sat Jul 14 22:37:53 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#ifndef DENTATA_SPRITE_H
#define DENTATA_SPRITE_H

typedef void d_sprite_t;

/**
 * d_sprite_new(void)
 * Creates a new sprite.
 *
 * Returns: a pointer to the newly created sprite.
 */
extern d_sprite_t *d_sprite_new(void);

/**
 * d_sprite_delete(d_sprite_t *spr)
 * Deletes a sprite and any space used by its underlying components.
 *
 * Takes: spr - the sprite to delete.
 */
extern void d_sprite_delete(d_sprite_t *);

/**
 * d_sprite_dup(d_sprite_t *spr)
 * Makes a deep copy of the sprite.
 *
 * Takes: spr - the sprite to duplicate.
 *
 * Returns: a copy of the sprite.
 */
extern d_sprite_t *d_sprite_dup(d_sprite_t *);

/**
 * d_sprite_addanim(d_sprite_t *spr)
 * Adds an animation to the sprite. Animations are numbered
 * sequentially.
 *
 * Takes: spr - the sprite to affect.
 *
 * Returns: the number associated with this animation, or -1 on failure.
 */
extern int d_sprite_addanim(d_sprite_t *);

/**
 * d_sprite_addframe(d_sprite_t *spr, int anim, d_image_t *frame)
 * Adds a frame to an animation in a sprite. The frame added will
 * be deleted automatically when the sprite is deleted.
 *
 * Takes: spr - the sprite to affect.
 *        anim - the number of the animation.
 *        frame - the frame to insert.
 */
extern void d_sprite_addframe(d_sprite_t *, int, d_image_t *);

/**
 * d_sprite_setanimparameters(d_sprite_t *spr, int framerate)
 */
extern void d_sprite_setanimparameters(d_sprite_t *, int);
extern d_image_t *d_sprite_getcurframe(d_sprite_t *);
extern void d_sprite_setcurframe(d_sprite_t *, word);
extern void d_sprite_stepframe(d_sprite_t *);
extern void d_sprite_setcuranim(d_sprite_t *, word);

#endif

/* EOF sprite.h */
