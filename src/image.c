/* 
 * image.c
 * Created: Sun Feb 25 01:57:37 2001 by tek@wiw.org
 * Revised: Sun Feb 25 02:54:45 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta image module
 */

#include <dentata/image.h>
#include <dentata/memory.h>
#include <dentata/error.h>
#include <dentata/primitives.h>

d_image_t *d_image_new(d_rasterdescription_t desc);
void       d_image_delete(d_image_t *p);
d_image_t *d_image_dup(d_image_t *p);
void       d_image_plot(d_image_t *p, d_point_t pt, d_color_t c);
void       d_image_wipe(d_image_t *p, d_color_t c);

d_image_t *d_image_new(d_rasterdescription_t desc)
{
    d_image_t *p;

    /* sanity checks */
    if(desc.w == 0 || desc.h == 0) {
        d_error_push("d_image_new: zero was given for an important dimension (width, height, or bpp).");
        return NULL;
    }

    p = d_memory_new(sizeof(d_image_t));
    if(p == NULL)
        return NULL;

    p->desc = desc;
    p->data = d_memory_new((desc.w*desc.h*desc.bpp+7)/8);
    p->alpha = d_memory_new((desc.w*desc.h*desc.alpha+7)/8);
    if(p->data == NULL ||
       (p->alpha == NULL && desc.alpha > 0))
        return NULL;

    return p;
}

void d_image_delete(d_image_t *p)
{
    d_memory_delete(p->data);
    if(p->alpha) d_memory_delete(p->alpha);
    d_memory_delete(p);
    return;
}

d_image_t *d_image_dup(d_image_t *p)
{
    d_image_t *q;

    q = d_image_new(p->desc);
    if(q == NULL) return NULL;

    d_memory_copy(q->data, p->data, (q->desc.w*q->desc.h*q->desc.bpp+7)/8);
    d_memory_copy(q->alpha, p->alpha, (q->desc.w*q->desc.h*q->desc.alpha+7)/8);

    q->palette = p->palette;

    return q;
}

void d_image_plot(d_image_t *p, d_point_t pt, d_color_t c)
{
    /* coordinates are unsigned, so we don't test for < 0. if this
       changes, however, a test should be performed */
    if(pt.x >= p->desc.w ||
       pt.y >= p->desc.h)
        return;

    switch(p->desc.bpp) {
    case 8:
        p->data[pt.x+pt.y*p->desc.w] = c;
        break;
        
    /* FIXME: I'm unsure about packing order here and below */
    case 15: /* fall thru */
    case 16:
        p->data[pt.x*2+pt.y*p->desc.w*2+0] = c&0xFF;
        p->data[pt.x*2+pt.y*p->desc.w*2+1] = c>>8;
        break;

    case 24:
        p->data[pt.x*3+pt.y*p->desc.w*3+0] = c&0xFF;
        p->data[pt.x*3+pt.y*p->desc.w*3+1] = (c>>8)&0xFF;
        p->data[pt.x*3+pt.y*p->desc.w*3+2] = (c>>16)&0xFF;
        break;

    case 32:
        p->data[pt.x*4+pt.y*p->desc.w*4+0] = c&0xFF;
        p->data[pt.x*4+pt.y*p->desc.w*4+1] = (c>>8)&0xFF;
        p->data[pt.x*4+pt.y*p->desc.w*4+2] = (c>>16)&0xFF;
        break;

    default:
        d_error_push("d_image_plot: unsupport bpp (report this as a bug).");
        return;
    }

    switch(p->desc.alpha) {
    case 0:
        break;

    case 1:
        p->alpha[pt.x/8+pt.y*p->desc.w/8] |= 1<<(pt.x%8);
        break;

    case 8:
        p->alpha[pt.x+pt.y*p->desc.w] = 0xFF;
        break;

    default:
        d_error_push("d_image_plot: unsupport alpha (report this as a bug).");
        return;
    }

    return;
}

void d_image_wipe(d_image_t *p, d_color_t c)
{
    int i;

    switch(p->desc.bpp) {
    case 8:
        d_memory_set(p->data, c, p->desc.h*p->desc.w);
        break;
        
    /* FIXME: I'm unsure about packing order here and below */
    case 15: /* fall thru */
    case 16:
        for(i = p->desc.w*p->desc.h*2; i >= 0;) {
            p->data[i--] = c>>8;
            p->data[i--] = c;
        }
        break;

    case 24:
        for(i = p->desc.w*p->desc.h*3; i >= 0;) {
            p->data[i--] = c>>16;
            p->data[i--] = c>>8;
            p->data[i--] = c;
        }
        break;

    case 32:
        for(i = p->desc.w*p->desc.h*4; i >= 0;) {
            p->data[i--] = c>>16;
            p->data[i--] = c>>8;
            p->data[i--] = c;
            i--;
        }
        break;


    default:
        d_error_push("d_image_wipe: unsupport bpp (report this as a bug).");
        return;
    }

    switch(p->desc.alpha) {
    case 0:
        break;

    case 1:
        d_memory_set(p->alpha, 0xFF, (p->desc.h*p->desc.w+7)/8);
        break;

    case 8:
        d_memory_set(p->alpha, 0xFF, p->desc.h*p->desc.w);
        break;

    default:
        d_error_push("d_image_wipe: unsupport alpha (report this as a bug).");
        return;
    }

    return;
}


/* EOF image.c */
