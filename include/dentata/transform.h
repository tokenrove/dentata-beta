/* 
 * transform.h
 * Created: Mon Jan 29 11:49:56 2001 by tek@wiw.org
 * Revised: Mon Jan 29 11:53:03 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#ifndef DENTATA_TRANSFORM_H
#define DENTATA_TRANSFORM_H

#include <dentata/image.h>

extern void      d_transform_scale(d_image_t *, word, word);
extern d_anim_t *d_transform_morph(d_image_t *, d_image_t *, byte);

#endif

/* EOF transform.h */
