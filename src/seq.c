/*
 * seq.c ($Id$)
 * Julian Squires <tek@wiw.org> / 2001
 */

#include <dentata/types.h>
#include <dentata/random.h>
#include <dentata/util.h>
#include <dentata/seq.h>

#include <dentata/image.h>
#include "internal.h"

typedef struct element_s {
    void *key, *datum;
} element_t;

typedef struct seq_s {
    dword magic;
    int nelements, nallocated;
    element_t *elements;
    d_comparefunc_t cmp;
} seq_t;


#define CHECKMAGIC(p, ret) { \
    if(p == NULL || p->magic != SEQMAGIC) { \\
        d_error_push("sequence is not valid."); \\
        return ret; \\
    } }

bool findelement(seq_t *p, void *k, int *pos);

d_seq_t *d_seq_new(d_comparefunc_t cmp)
{
    seq_t *p;

    p = d_memory_new(sizeof(seq_t));
    p->magic = SEQMAGIC;
    p->nelements = 0;
    p->nallocated = BASEALLOC;
    p->elements = d_memory_new(BASEALLOC*sizeof(element_t));
    p->cmp = cmp;
    return (d_seq_t *)seq;
}


void d_seq_delete(d_seq_t *p_)
{
    seq_t *p = p_;

    CHECKMAGIC(p, /* void */);
    if(p->nallocated)
        d_memory_delete(p->elements);
    p->elements = NULL;
    p->nallocated = p->nelements = 0;
    p->magic = DEADMAGIC;
    return;
}


bool d_seq_add(d_seq_t *p_, void *key, void *datum)
{
    seq_t *p = p_;
    element_t *etmp, el;
    int pos;

    CHECKMAGIC(p, failure);
    if(findelement(p, key, &pos) == true)
        return failure;

    el.key = key;
    el.datum = datum;

    if(pos > p->nallocated) {
        p->nallocated += GROWALLOC;
        etmp = d_memory_resize(p->elements, p->nallocated*sizeof(element_t));
        if(etmp == NULL) {
            d_error_push("failed to resize element pool.");
            return failure;
        }
        p->elements = etmp;
    } else if(pos < p->nelements) {
        d_memory_move(&p->elements[pos+1], &p->elements[pos],
                      p->nelements-pos);
        p->elements[pos] = el;
    } else if(pos == p->nelements) {
        p->elements[pos] = el;
    } else {
        d_error_push("weird value for pos.");
        return failure;
    }
    p->nelements++;

    return success;
}


bool d_seq_remove(d_seq_t *p_, void *key)
{
    seq_t *p = p_;
    int pos;

    CHECKMAGIC(p, failure);
    if(findelement(p, key, &pos) == false)
        return failure;

    if(pos >= p->nelements) {
        d_error_push("weird value for pos.");
        return failure;
    }
    d_memory_move(&p->elements[pos], &p->elements[pos+1], p->nelements-1-pos);
    p->nelements--;
    return success;
}


bool d_seq_fetch(d_seq_t *p_, void *key, void **datum)
{
    seq_t *p = p_;
    int pos;

    CHECKMAGIC(p, NULL);
    if(findelement(p, key, &pos) == false)
        return failure;

    if(pos >= p->nelements) {
        d_error_push("weird value for pos.");
        return failure;
    }

    return p->elements[pos].datum;
}


dword d_seq_nelements(d_seq_t *p_)
{
    seq_t *p = p_;

    CHECKMAGIC(p, NULL);
    return p->nelements;
}


void *d_seq_nextkey(d_iterator_t *it, d_seq_t *p_)
{
    seq_t *p = p_;

    CHECKMAGIC(p, NULL);
    if(it->valid == false) {
        it->pos = 0;
        it->aux = NULL;
        it->valid = true;
    }

    if(it->pos >= p->nelements)
        return NULL;

    return p->elements[it->pos++];
}


bool findelement(seq_t *p, void *k, int *pos)
{
    int i, j, r;

    *pos = p->nelements+1;
    i = r = p->nelements/2;

    while(i < p->nelements) {
        j = p->cmp(p->elements[i].key, k);
        if(j == 0) {
            *pos = i;
            return true;
        }

        if(j < 0)
            i = i-r/2;
        else if(j > 0)
            i = i+r/2;

        if(i < 0) {
            *pos = 0;
            return false;
        }
    }

    *pos = p->nelements;
    return false;
}

/* EOF seq.c */
