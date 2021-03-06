/**
 * util.h
 * Created: Thu Apr 19 04:30:20 2001 by tek@wiw.org
 * Revised: Sun Jun 24 06:55:55 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: util
 *
 * Include dentata/types.h and dentata/random.h before dentata/util.h.
 */

#ifndef DENTATA_UTIL_H
#define DENTATA_UTIL_H

#include <stdarg.h>

/**
 * d_point_center(d_rect_t r)
 * Gets the center of a rectangle.
 *
 * Takes: r - the rectangle.
 *
 * Returns: the coordinates of the point in the center.
 */
extern d_point_t d_point_center(d_rect_t);

/**
 * d_point_origin(void)
 * Returns the point at the origin, (0,0).
 *
 * Returns: the coordinates of the point at the origin.
 */
extern d_point_t d_point_origin(void);

/**
 * d_point_random(d_random_t *rh, d_rect_t r)
 * Generates a random point inside the specified rectangle.
 *
 * Takes: rh - the random handle to use.
 *        r - the rectangle.
 *
 * Returns: the coordinates of the randomly generated point.
 */
extern d_point_t d_point_random(d_random_t *, d_rect_t);

/**
 * d_iterator_reset(d_iterator_t *it)
 * Resets the iteration counter. Note that this should be called before
 * any calls to d_set_nextkey() if the set has been changed in any way
 * since the last time it was called.
 *
 * Takes: it - pointer to the iterator.
 */
extern void d_iterator_reset(d_iterator_t *);

/**
 * d_util_vprintflen(byte *fmt, va_list ap)
 * Determines how many bytes will be required to hold a formatted string.
 *
 * Takes: fmt - the format string, as printf(3).
 *        argptr - the address of the first argument after the format
 *                 string.
 *
 * Returns: the length of the formatted string.
 */
extern int d_util_vprintflen(byte *, va_list);

/***
 * d_util_sprintf(byte *dst, byte *fmt, va_list ap)
 * Formats a string, similar to sprintf(3).
 *
 * Takes: dst - the destination string.
 *        fmt - the format string.
 *        argptr - the address of the first argument after the format
 *                 string.
 */
extern void d_util_vsprintf(byte *, byte *, va_list);

#endif

/* EOF util.h */
