/* 
 * set.c
 * Created: Sat Feb 24 21:37:59 2001 by tek@wiw.org
 * Revised: Fri Jul 27 03:54:39 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta set data structure - implemented as a hash table.
 *
 * FIXME: Should intelligently grow the table when average list depth is
 * greater than two or so.
 */

#include <dentata/types.h>
#include <dentata/set.h>
#include <dentata/memory.h>
#include <dentata/error.h>

#define MULTIPLIER 31
#define DEFTABLESIZE 256

#include <dentata/image.h>
#include "internal.h"

/* internal set structures */
typedef struct element_s {
    dword key;
    void *data;
    struct element_s *next;
} element_t;

typedef struct set_s {
    dword magic;
    int nelements, tablesize;
    element_t **elements;
} set_t;


#define CHECKMAGIC(p, ret) { \
    if(p == NULL || p->magic != SETMAGIC) { \
        d_error_push(__FUNCTION__": set is not valid."); \
        return ret; \
    } }


d_set_t *d_set_new(int sizehint);
void d_set_delete(d_set_t *p);
bool d_set_add(d_set_t *p, dword key, void *data);
bool d_set_remove(d_set_t *p, dword key);
bool d_set_fetch(d_set_t *p, dword key, void **data);
dword d_set_getunusedkey(d_set_t *p);
int d_set_nelements(d_set_t *p);
dword d_set_nextkey(d_iterator_t *it, d_set_t *p);
/* internal functions */
static dword hash(set_t *p, dword key);
static bool chainadd(element_t **ep, dword key, void *data);
static element_t *chainfind(element_t *ep, dword key);
static bool chainremove(element_t **ep, dword key);


d_set_t *d_set_new(int sizehint)
{
    set_t *p;

    p = d_memory_new(sizeof(set_t));
    if(p == NULL)
        return NULL;

    p->tablesize = DEFTABLESIZE;
    if(sizehint > 0) {
        /* This is an arbitrary heuristic - a possibility for the future
           is examining better heuristics for this purpose */
        if(p->tablesize/2 > sizehint ||
           p->tablesize*2 < sizehint) {
            p->tablesize = sizehint;
        }
    }
    p->elements = d_memory_new(sizeof(element_t *)*p->tablesize);
    if(p->elements == NULL)
        return NULL;
    d_memory_set(p->elements, 0, sizeof(element_t *)*p->tablesize);
    p->nelements = 0;

    p->magic = SETMAGIC;

    return (void *)p;
}


void d_set_delete(d_set_t *p_)
{
    set_t *p = (set_t *)p_;
    element_t *ep;
    int i;

    CHECKMAGIC(p, /* void */);

    p->magic = 0;
    for(i = 0; i < p->tablesize; i++) {
        while(p->elements[i] != NULL) {
            ep = p->elements[i];
            p->elements[i] = ep->next;
            d_memory_delete(ep);
        }
    }
    d_memory_delete(p->elements);
    d_memory_delete(p);
    return;
}


bool d_set_add(d_set_t *p_, dword key, void *data)
{
    set_t *p = (set_t *)p_;
    bool ret;

    CHECKMAGIC(p, failure);

    if(key == D_SET_INVALIDKEY)
        return failure;

    ret = chainadd(&p->elements[hash(p, key)], key, data);
    if(ret == success) {
        p->nelements++;
    }

    return ret;
}


bool d_set_remove(d_set_t *p_, dword key)
{
    set_t *p = (set_t *)p_;
    bool ret;

    CHECKMAGIC(p, failure);

    if(key == D_SET_INVALIDKEY)
        return failure;

    ret = chainremove(&p->elements[hash(p, key)], key);
    if(ret == success) {
        p->nelements--;
        if(p->nelements < 0)
            d_error_fatal("d_set_remove: p->nelements is wrong!");
    }

    return ret;
}


bool d_set_fetch(d_set_t *p_, dword key, void **data)
{
    set_t *p = (set_t *)p_;
    element_t *ep;

    CHECKMAGIC(p, failure);

    if(key == D_SET_INVALIDKEY)
        return failure;

    ep = chainfind(p->elements[hash(p, key)], key);
    if(ep == NULL)
        return failure;
    
    if(data != NULL)
        *data = ep->data;
    return success;
}


dword d_set_getunusedkey(d_set_t *p_)
{
    set_t *p = (set_t *)p_;
    int i;

    CHECKMAGIC(p, D_SET_INVALIDKEY);

    for(i = 0; i < p->tablesize; i++)
        if(chainfind(p->elements[hash(p, i)], i) == failure)
            return i;

    /* There are no valid keys remaining without collisions.
       Try and find some key. FIXME: This should try and find
       a key which hashes to the least-deep bucket. */
    for(; i < D_SET_INVALIDKEY; i++)
        if(chainfind(p->elements[hash(p, i)], i) == failure)
            return i;

    d_error_push("d_set_getunusedkey: no free keyspace exists.");
    return D_SET_INVALIDKEY;
}

int d_set_nelements(d_set_t *p_)
{
    set_t *p = (set_t *)p_;

    CHECKMAGIC(p, -1);

    return p->nelements;
}


dword d_set_nextkey(d_iterator_t *it, d_set_t *p_)
{
    set_t *p = (set_t *)p_;
    element_t *ep;

    CHECKMAGIC(p, D_SET_INVALIDKEY);

    if(it->signature != SETMAGIC) {
        it->pos = 0;
        it->aux = NULL;
        it->signature = SETMAGIC;
    }

    if(it->aux != NULL) {
        ep = (element_t *)it->aux;
        it->aux = ep->next;
        if(ep->next != NULL)
            return ep->next->key;
    }

    for(; it->pos < p->tablesize; it->pos++) {
        if(p->elements[it->pos] == NULL)
            continue;
        ep = p->elements[it->pos++];
        it->aux = ep;
        return ep->key;
    }

    return D_SET_INVALIDKEY;
}


dword hash(set_t *p, dword key)
{
    return (MULTIPLIER*key)%p->tablesize;
}


bool chainadd(element_t **ep, dword key, void *data)
{
    while(*ep != NULL) {
        if((*ep)->key == key)
            return failure;
        ep = &(*ep)->next;
    }

    *ep = d_memory_new(sizeof(element_t));
    if(*ep == NULL)
        return failure;
    (*ep)->key = key;
    (*ep)->data = data;
    (*ep)->next = NULL;

    return success;
}


bool chainremove(element_t **ep, dword key)
{
    element_t *deadelement;

    while(*ep != NULL) {
        if((*ep)->key == key) {
            deadelement = *ep;
            *ep = deadelement->next;
            d_memory_delete(deadelement);
            return success;
        }
        ep = &(*ep)->next;
    }

    return failure;
}


element_t *chainfind(element_t *ep, dword key)
{
    while(ep != NULL) {
        if(ep->key == key)
            return ep;
        ep = ep->next;
    }
    
    return NULL;
}

/* EOF set.c */
