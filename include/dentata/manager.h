/* 
 * manager.h
 * Created: Sun Jul 15 01:06:26 2001 by tek@wiw.org
 * Revised: Fri Jul 20 01:49:41 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#ifndef DENTATA_MANAGER_H
#define DENTATA_MANAGER_H

extern bool d_manager_new(void);
extern void d_manager_delete(void);
extern word d_manager_getmaxlayers(void);
extern word d_manager_getmaxsprites(void);
extern bool d_manager_addsprite(d_sprite_t *, word *, char);
extern bool d_manager_removesprite(word);
extern bool d_manager_addimagelayer(d_image_t *, word *, char);
extern bool d_manager_addtilemaplayer(d_tilemap_t *, word *, char);
extern void d_manager_wipelayers(void);
extern void d_manager_wipesprites(void);
extern void d_manager_draw(d_image_t *);
extern void d_manager_pansprite(word, int, int);
extern void d_manager_jumpsprite(word, word, word);
extern void d_manager_setscrollparameters(bool, char);
extern void d_manager_pan(int, int);
extern void d_manager_jump(int, int);
extern d_rect_t d_manager_getvirtualrect(void);

#endif

/* EOF manager.h */
