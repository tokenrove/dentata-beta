/* 
 * util.c
 * Created: Thu Apr 19 06:36:50 2001 by tek@wiw.org
 * Revised: Sat Apr 28 16:28:37 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * gen beta utility functions
 *
 */

#include <dentata/types.h>
#include <dentata/random.h>
#include <dentata/util.h>

d_point_t d_point_center(d_rect_t r)
{
    d_point_t pt;

    pt.x = r.w/2+r.offset.x;
    pt.y = r.h/2+r.offset.y;
    return pt;
}

d_point_t d_point_origin(void)
{
    d_point_t pt = { 0, 0 };

    return pt;
}

/* EOF util.c */
