/* 
 * stdlib.c
 * Created: Mon Jan  8 04:19:39 2001 by tek@wiw.org
 * Revised: Thu Apr 19 04:24:22 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * Memory module, stdlib wrapper version
 */

#include <dentata/types.h>
#include <dentata/memory.h>

#include <stdlib.h>
#include <string.h>

void *d_memory_new(dword);
void  d_memory_delete(void *);
void *d_memory_resize(void *, dword);

void  d_memory_copy(void *, void *, dword);
void  d_memory_move(void *, void *, dword);
void  d_memory_set(void *, byte, dword);


void *d_memory_new(dword len)
{
    return malloc(len);
}

void d_memory_delete(void *p)
{
    free(p);
}

void *d_memory_resize(void *p, dword len)
{
    return realloc(p, len);
}


void d_memory_copy(void *d, void *s, dword len)
{
    memcpy(d, s, len);
}

void d_memory_move(void *d, void *s, dword len)
{
    memcpy(d, s, len);
}

void d_memory_set(void *d, byte v, dword len)
{
    memset(d, v, len);
}

/* EOF stdlib.c */
