/**
 * util.h
 * Created: Thu Apr 19 04:30:20 2001 by tek@wiw.org
 * Revised: Sat Jun 23 03:29:30 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: util
 *
 * Include dentata/types.h and dentata/random.h before this file.
 */

#ifndef DENTATA_UTIL_H
#define DENTATA_UTIL_H

extern d_point_t d_point_center(d_rect_t);
extern d_point_t d_point_origin(void);
extern d_point_t d_point_random(d_random_t *, d_rect_t);
extern int d_util_printflen(byte *, void *);
extern void d_util_sprintf(byte *, byte *, void *);

#endif

/* EOF util.h */
