/* 
 * primitives.h
 * Created: Wed Jan 24 11:20:18 2001 by tek@wiw.org
 * Revised: Thu Apr 12 03:46:05 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta
 * Primitives module
 */

#ifndef DENTATA_PRIMITIVES_H
#define DENTATA_PRIMITIVES_H

#include <dentata/types.h>
#include <dentata/random.h>

typedef struct d_point_s {
    int x, y;
} d_point_t;

typedef struct d_rect_s {
    d_point_t offset;
    word w, h;
} d_rect_t;

extern d_point_t d_point_center(d_rect_t);
extern d_point_t d_point_random(d_random_t *, d_rect_t);

#endif

/* EOF primitives.h */
