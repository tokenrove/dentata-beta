/* 
 * spritgen.c
 * Created: Sat Jul 14 22:29:09 2001 by tek@wiw.org
 * Revised: Thu Jul 19 18:52:02 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>
#include <dentata/sprite.h>
#include <dentata/memory.h>
#include <dentata/error.h>

#include "internal.h"

/* private sprite structure */
typedef struct sprite_s {
    dword magic;
    word nanims, curanim;
    word *nframes, curframe, framelag;
    d_image_t ***frames;
} sprite_t;

#ifdef DEBUG
#define MAGICCHECK(p, ret) { \
    if(p == NULL) { \
        d_error_push(__FUNCTION__": Passed a NULL pointer."); \
        return ret; \
    } \
    if(p->magic != SPRITEMAGIC) { \
        d_error_push(__FUNCTION__": Bad sprite magic."); \
        return ret; \
    } \
}
#else
#define MAGICCHECK(p, ret) { }
#endif

d_sprite_t *d_sprite_new(void);
void d_sprite_delete(d_sprite_t *);
d_sprite_t *d_sprite_dup(d_sprite_t *);
int d_sprite_addanim(d_sprite_t *);
void d_sprite_addframe(d_sprite_t *, int, d_image_t *);
void d_sprite_setanimparameters(d_sprite_t *, int);
d_image_t *d_sprite_getcurframe(d_sprite_t *);
void d_sprite_setcurframe(d_sprite_t *, word);
void d_sprite_stepframe(d_sprite_t *);
void d_sprite_setcuranim(d_sprite_t *, word);


d_sprite_t *d_sprite_new(void)
{
    sprite_t *p;

    p = d_memory_new(sizeof(sprite_t));
    if(p == NULL) {
        d_error_push(__FUNCTION__": failed to allocate sprite structure.");
        return NULL;
    }
    p->magic = SPRITEMAGIC;
    p->curanim = 0;
    p->curframe = 0;
    p->framelag = 1;
    p->nanims = 0;
    p->frames = NULL;
    p->nframes = NULL;

    return (d_sprite_t *)p;
}

void d_sprite_delete(d_sprite_t *p_)
{
    sprite_t *p = p_;
    int i, j;

    MAGICCHECK(p, /* void */);
    for(i = 0; i < p->nanims; i++) {
        for(j = 0; j < p->nframes[i]; j++) {
            d_image_delete(p->frames[i][j]);
            p->frames[i][j] = NULL;
        }
        d_memory_delete(p->frames[i]);
        p->frames[i] = NULL;
    }
    d_memory_delete(p->nframes);
    p->nframes = NULL;
    d_memory_delete(p->frames);
    p->frames = NULL;
    p->magic = DEADMAGIC;
    return;
}

d_sprite_t *d_sprite_dup(d_sprite_t *p_)
{
    d_sprite_t *q_;
    sprite_t *q, *p;
    int i, j;

    p = p_;
    MAGICCHECK(p, NULL);

    q_ = d_sprite_new();
    q = q_;
    q->nanims = p->nanims;
    q->frames = d_memory_new(q->nanims*sizeof(d_image_t **));
    if(q->frames == NULL)
        return NULL;
    q->nframes = d_memory_new(q->nanims*sizeof(word));
    if(q->nframes == NULL)
        return NULL;

    for(i = 0; i < q->nanims; i++) {
        q->frames[i] = d_memory_new(q->nframes[i]*sizeof(d_image_t *));
        if(q->frames[i] == NULL)
            return NULL;
        q->nframes[i] = p->nframes[i];

        for(j = 0; j < q->nframes[i]; j++) {
            q->frames[i][j] = d_image_dup(p->frames[i][j]);
            if(q->frames[i][j] == NULL)
                return NULL;
        }
    }

    q->curanim = p->curanim;
    q->curframe = p->curframe;
    q->framelag = p->framelag;

    return q_;
}

int d_sprite_addanim(d_sprite_t *p_)
{
    sprite_t *p;

    p = p_;
    MAGICCHECK(p, -1);

    p->nanims++;
    p->nframes = d_memory_resize(p->nframes, p->nanims*sizeof(word));
    p->nframes[p->nanims-1] = 0;
    p->frames = d_memory_resize(p->frames, p->nanims*sizeof(d_image_t **));
    p->frames[p->nanims-1] = NULL;
    return p->nanims-1;
}

void d_sprite_addframe(d_sprite_t *p_, int anim, d_image_t *frame)
{
    sprite_t *p;

    p = p_;
    MAGICCHECK(p, /* void */);
    if(anim >= p->nanims) {
        d_error_push(__FUNCTION__": invalid animation.");
        return;
    }

    p->nframes[anim]++;
    p->frames[anim] = d_memory_resize(p->frames[anim],
                                      p->nframes[anim]*sizeof(d_image_t *));
    p->frames[anim][p->nframes[anim]-1] = frame;
    return;
}

void d_sprite_setanimparameters(d_sprite_t *p_, int framelag)
{
    sprite_t *p;

    p = p_;
    MAGICCHECK(p, /* void */);
    if(framelag < 1)
        return;
    p->framelag = framelag;
    return;
}

d_image_t *d_sprite_getcurframe(d_sprite_t *p_)
{
    sprite_t *p;

    p = p_;
    MAGICCHECK(p, NULL);
    return p->frames[p->curanim][p->curframe/p->framelag];
}

void d_sprite_setcurframe(d_sprite_t *p_, word curframe)
{
    sprite_t *p;

    p = p_;
    MAGICCHECK(p, /* void */);
    p->curframe = curframe*p->framelag;
    return;
}

void d_sprite_stepframe(d_sprite_t *p_)
{
    sprite_t *p;

    p = p_;
    MAGICCHECK(p, /* void */);
    p->curframe++;
    if(p->curframe/p->framelag >= p->nframes[p->curanim])
        p->curframe = 0;
    return;
}

void d_sprite_setcuranim(d_sprite_t *p_, word curanim)
{
    sprite_t *p;

    p = p_;
    MAGICCHECK(p, /* void */);
    if(curanim >= p->nanims) {
        d_error_push(__FUNCTION__": invalid animation.");
        return;
    }

    if(curanim != p->curanim)
        p->curframe = 0;
    p->curanim = curanim;
    return;
}

/* EOF spritgen.c */
