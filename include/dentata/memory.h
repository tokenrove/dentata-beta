/**
 * memory.h
 * Created: Sat Jan  8 23:49:44 2000 by tek@wiw.org
 * Revised: Sun Jun 24 01:06:13 2001 by tek@wiw.org
 * Copyright 2000 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: memory
 *
 * Include dentata/types.h before dentata/memory.h.
 */

#ifndef DENTATA_MEMORY_H
#define DENTATA_MEMORY_H

/**
 * d_memory_new(dword nbytes)
 * Allocates a chunk of memory, as malloc(3).
 *
 * Takes: nbytes - number of bytes to allocate.
 *
 * Returns: a pointer to the freshly allocated memory, or NULL if
 *          something went wrong.
 */
extern void *d_memory_new(dword);

/**
 * d_memory_delete(void *p)
 * Free a chunk of previously allocated memory, as free(3).
 *
 * Takes: p - the pointer to the allocated memory to free.
 */
extern void  d_memory_delete(void *);

/**
 * d_memory_resize(void *p, dword nbytes)
 * Attempts to change the amount of memory allocated at the supplied
 * pointer, as realloc(3). Behavior is as POSIX realloc(3).
 *
 * Takes: p - the pointer to the allocated memory to resize.
 *        nbytes - the new number of bytes to allocate.
 *
 * Returns: a pointer to the resized memory area, or NULL if something
 *          went wrong. (p is not deallocated in this case)
 */
extern void *d_memory_resize(void *, dword);

/**
 * d_memory_copy(void *dst, void *src, dword nbytes)
 * Copies a contiguous sequence of bytes from the source address to
 * the destination address, as memcpy(3).
 *
 * Takes: dst - the destination address.
 *        src - the source address.
 *        nbytes - the number of bytes to copy.
 */
extern void  d_memory_copy(void *, const void *, dword);

/**
 * d_memory_move(void *dst, void *src, dword nbytes)
 * Moves a contiguous sequence of bytes from the source address to
 * the destination address, as memmove(3). The destinations may
 * overlap.
 *
 * Takes: dst - the destination address.
 *        src - the source address.
 *        nbytes - the number of bytes to copy.
 */
extern void  d_memory_move(void *, const void *, dword);

/**
 * d_memory_set(void *dst, byte value, dword nbytes)
 * Sets a contiguous sequence of bytes with the supplied value, as
 * memset(3).
 *
 * Takes: dst - the destination address.
 *        value - the value with which to fill.
 *        nbytes - the number of bytes to copy.
 */
extern void  d_memory_set(void *, byte, dword);

#endif

/* EOF memory.h */
