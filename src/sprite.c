/* 
 * sprite.c
 * Created: Mon Feb  5 15:16:46 2001 by tek@wiw.org
 * Revised: Mon Feb  5 15:16:46 2001 (pending)
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/error.h>
#include <dentata/file.h>
#include <dentata/memory.h>
#include <dentata/image.h>
#include <dentata/anim.h>
#include <dentata/sprite.h>
#include <dentata/pcx.h>
#include <dentata/tga.h>

d_sprite_t *d_sprite_new(void);
d_sprite_t *d_sprite_dup(d_sprite_t *);
void        d_sprite_delete(d_sprite_t *);
d_sprite_t *d_sprite_loadfromspr(char *);

void        d_sprite_addanim(d_sprite_t *, d_anim_t *);
word        d_sprite_curwidth(d_sprite_t *);
word        d_sprite_curheight(d_sprite_t *);

d_sprite_t *d_sprite_new(void)
{
    d_sprite_t *p;

    p = d_memory_new(sizeof(d_sprite_t));
    p->animated = false;
    p->nanims = 0;
    p->curanim = 0;
    p->anim = NULL;
    p->pos.x = 0;
    p->pos.y = 0;
    p->clip.offset = p->pos;
    p->clip.w = 0;
    p->clip.h = 0;
    p->clipstrategy = none;
    return p;
}

d_sprite_t *d_sprite_dup(d_sprite_t *p)
{
    d_sprite_t *q;
    int i;

    q = d_memory_new(sizeof(d_sprite_t));
    q->animated = p->animated;
    q->nanims = p->nanims;
    q->curanim = p->curanim;
    q->anim = d_memory_new(sizeof(d_anim_t *)*q->nanims);
    for(i = 0; i < q->nanims; i++)
        q->anim[i] = d_anim_dup(p->anim[i]);
    q->pos = p->pos;
    q->clip = p->clip;
    q->clipstrategy = p->clipstrategy;
    return q;
}

void d_sprite_delete(d_sprite_t *p)
{
    int i;

    for(i = 0; i < p->nanims; i++)
        d_anim_delete(p->anim[i]);
    d_memory_delete(p);
    return;
}

d_sprite_t *d_sprite_loadfromspr(char *filename)
{
    d_sprite_t *p;
    d_file_t *file;
    int i, j, k;
    char *s;

    p = d_sprite_new();
    file = d_file_open(filename);
    p->pos.x = d_file_getword(file);
    p->pos.y = d_file_getword(file);
    p->clipstrategy = d_file_getbyte(file);
    p->clip.offset.x = d_file_getword(file);
    p->clip.offset.y = d_file_getword(file);
    p->clip.w = d_file_getword(file);
    p->clip.h = d_file_getword(file);
    p->animated = true;
    p->nanims = d_file_getword(file);
    p->anim = d_memory_new(p->nanims*sizeof(d_anim_t));
    for(i = 0; i < p->nanims; i++) {
        p->anim[i] = d_anim_new((byte)d_file_getword(file));
        for(j = d_file_getword(file); j >= 0; j--) {
            k = d_file_getbyte(file);
            s = d_file_getasciizstring(file);

            switch(k) {
            case 0:
                d_error_push("d_sprite_loadfromspr: pnm is no longer "
                             "supported.");
                return NULL;
            case 1:
                d_anim_addframe(p->anim[i], d_pcx_load(s));
                break;
            case 2:
                d_anim_addframe(p->anim[i], d_tga_load(s));
                break;
            default:
                d_error_push("d_sprite_loadfromspr: unrecognized format "
                             "byte.");
                return NULL;
            }

            d_memory_delete(s);
        }
    }

    d_file_close(file);
    return p;
}

void d_sprite_addanim(d_sprite_t *p, d_anim_t *a)
{
    p->nanims++;
    p->anim = d_memory_resize(p->anim, sizeof(d_anim_t *)*p->nanims);
    p->anim[p->nanims-1] = a;
    return;
}

word d_sprite_curwidth(d_sprite_t *p)
{
    word t;

    t = p->anim[p->curanim]->curframe;
    return p->anim[p->curanim]->frames[t]->desc.w;
}

word d_sprite_curheight(d_sprite_t *p)
{
    word t;

    t = p->anim[p->curanim]->curframe;
    return p->anim[p->curanim]->frames[t]->desc.h;
}

/* EOF sprite.c */
