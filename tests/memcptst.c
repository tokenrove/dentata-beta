/* 
 * memcptst.c
 * Created: Sun Jun 24 01:36:25 2001 by tek@wiw.org
 * Revised: Sun Jun 24 11:09:56 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/memory.h>
#include <dentata/error.h>

/* 79561 is prime and not evenly divisible by four (mod 4 == 1) */
/* #define TESTSIZE 79561 */
/* 339827 is another good one (mod 4 == 3) */
#define TESTSIZE 339827

int main(void)
{
    byte *a, *b, v = 0xCC;
    int i, n = TESTSIZE;

    a = d_memory_new(n);
    b = a+n/3;

    /* memset test 1 */
    d_memory_set(a, v-1, n);
    d_memory_set(a, v, n);
    for(i = 0; i < n; i++)
        if(a[i] != v)
            d_error_fatal("d_memory_set test 1a failed!\n");

    /* memcpy test 1 */
    d_memory_set(b, v-1, n/2);
    d_memory_copy(b, a, n/2);
    for(i = 0; i < n/2; i++)
        if(b[i] != v)
            d_error_fatal("d_memory_copy test 1a failed!\n");

    /* memmove test 1 */
    d_memory_set(b, v-1, n/2);
    d_memory_move(a, b, n/2);
    for(i = 0; i < n/2; i++)
        if(a[i] != v-1)
            d_error_fatal("d_memory_move test 1b failed!\n");
    
    d_memory_delete(a);
    return 0;
}

/* EOF memcptst.c */
