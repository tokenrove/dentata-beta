/* 
 * memory.h
 * Created: Sat Jan  8 23:49:44 2000 by tek@wiw.org
 * Revised: Wed Jan 24 11:09:58 2001 by tek@wiw.org
 * Copyright 2000 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * Memory management module
 */

#include <dentata/types.h>

#ifndef DENTATA_MEMORY_H
#define DENTATA_MEMORY_H

extern void *d_memory_new(dword);
extern void  d_memory_delete(void *);
extern void *d_memory_resize(void *, dword);

extern void  d_memory_copy(void *, void *, dword);
extern void  d_memory_move(void *, void *, dword);

#endif

/* EOF memory.h */
