/* 
 * layermediator.h
 * Created: Sun Jan 28 15:51:49 2001 by tek@wiw.org
 * Revised: Mon Jan 29 13:08:11 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta
 * Layer mediator
 */

#ifndef DENTATA_LAYERMEDIATOR_H
#define DENTATA_LAYERMEDIATOR_H

extern bool       d_layermediator_new(void);
extern void       d_layermediator_delete(void);

extern word       d_layermediator_add(d_layer_t *, byte);
extern d_layer_t *d_layermediator_fetch(word);
extern void       d_layermediator_remove(word);
extern void       d_layermediator_draw(d_image_t *, d_rect_t);

extern void       d_layermediator_pan(word, int, int);
extern void       d_layermediator_jump(word, word, word);

#endif

/* EOF layermediator.h */
