/* 
 * internal.h
 * Created: Sat Apr 28 16:29:17 2001 by tek@wiw.org
 * Revised: Sat Apr 28 16:29:43 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#define SETMAGIC 0xDEDE1240
#define SEQMAGIC 0xDEDE1241

extern void __d_image_clip(d_image_t *, d_image_t *, d_point_t, dword *,
                           dword *, dword *, dword *, dword *, dword *);

/* EOF internal.h */
