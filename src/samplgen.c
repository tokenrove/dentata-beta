/* 
 * samplgen.c
 * Created: Sun Apr 15 04:11:42 2001 by tek@wiw.org
 * Revised: Sat Jun 23 04:12:03 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/sample.h>
#include <dentata/memory.h>

d_sample_t *d_sample_new(d_audiomode_t mode)
{
    d_sample_t *p;

    p = d_memory_new(sizeof(d_sample_t));
    if(p == NULL) return NULL;
    p->mode = mode;
    p->len = 0;
    p->data = NULL;
    p->hasloop = false;
    p->lbegin = p->lend = 0;
    return p;
}

void d_sample_delete(d_sample_t *p)
{
    if(p->data) d_memory_delete(p->data);
    d_memory_delete(p);
    return;
}

d_sample_t *d_sample_dup(d_sample_t *p)
{
    d_sample_t *q;

    if(p == NULL) return NULL;

    q = d_sample_new(p->mode);
    if(q == NULL) return NULL;
    q->len = p->len;
    q->data = d_memory_new(q->len);
    if(q->data == NULL && q->len > 0) return NULL;
    d_memory_copy(q->data, p->data, q->len);
    q->hasloop = p->hasloop;
    q->lbegin = p->lbegin;
    q->lend = p->lend;
    
    return q;
}

/* EOF samplgen.c */
