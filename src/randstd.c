/* 
 * randstd.c
 * Created: Sat May  5 09:25:59 2001 by tek@wiw.org
 * Revised: Sat Jun 23 04:11:06 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/random.h>

#include <stdlib.h>
#include <time.h>

d_random_t *d_random_new(void);
void d_random_delete(d_random_t *);
dword d_random_gen(d_random_t *, dword);

d_random_t *d_random_new(void)
{
    srand(time(0));
    return NULL;
}

void d_random_delete(d_random_t *r)
{
    return;
}

dword d_random_gen(d_random_t *r, dword max)
{
    return ((dword)rand())%max;
}

/* EOF randstd.c */
