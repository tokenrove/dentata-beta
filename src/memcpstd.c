/* 
 * memcpstd.c
 * Created: Sun Jun 24 01:07:02 2001 by tek@wiw.org
 * Revised: Sun Jun 24 01:08:14 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Memory module, string operations, stdlib wrapper version.
 */

#include <dentata/types.h>
#include <dentata/memory.h>
#include <string.h>

void d_memory_copy(void *, const void *, dword);
void d_memory_move(void *, const void *, dword);
void d_memory_set(void *, byte, dword);

void d_memory_copy(void *d, const void *s, dword len)
{
    memcpy(d, s, len);
}

void d_memory_move(void *d, const void *s, dword len)
{
    memmove(d, s, len);
}

void d_memory_set(void *d, byte v, dword len)
{
    memset(d, v, len);
}

/* EOF memcpstd.c */
