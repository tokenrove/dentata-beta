/* 
 * image.c
 * Created: Sun Feb 25 01:57:37 2001 by tek@wiw.org
 * Revised: Fri Apr 27 20:20:34 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta image module
 */

#include <dentata/types.h>
#include <dentata/image.h>
#include <dentata/color.h>
#include <dentata/memory.h>
#include <dentata/error.h>

d_image_t *d_image_new(d_rasterdescription_t desc);
void d_image_delete(d_image_t *p);
d_image_t *d_image_dup(d_image_t *p);
void d_image_plot(d_image_t *p, d_point_t pt, d_color_t c, byte alpha);
void d_image_wipe(d_image_t *p, d_color_t c, byte alpha);
void d_image_silhouette(d_image_t *image, d_color_t color, byte alpha);
bool d_image_extendalpha(d_image_t *p, byte alpha);
bool d_image_convertdepth(d_image_t *p, byte bpp);

d_image_t *d_image_new(d_rasterdescription_t desc)
{
    d_image_t *p;

    /* sanity checks */
    if(desc.w == 0 || desc.h == 0 || desc.bpp == 0) {
        d_error_push("d_image_new: zero was given for an important dimension (width, height, or bpp).");
        return NULL;
    }

    p = d_memory_new(sizeof(d_image_t));
    if(p == NULL)
        return NULL;

    p->desc = desc;
    p->data = d_memory_new((desc.w*desc.h*desc.bpp+7)/8);
    if(desc.alpha != 0)
        p->alpha = d_memory_new((desc.w*desc.h*desc.alpha+7)/8);
    else
        p->alpha = NULL;
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

void d_image_plot(d_image_t *p, d_point_t pt, d_color_t c, byte alpha)
{
    byte mask;
    int i, o;

    if(pt.x < 0 ||
       pt.y < 0 ||
       pt.x >= p->desc.w ||
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
    case 2:
    case 4:
    case 8:
        mask = (1<<p->desc.alpha)-1;
        alpha >>= (8-p->desc.alpha);
        o = pt.x+pt.y*p->desc.w;
        i = 8/p->desc.alpha;
        p->alpha[o/i] &= ~(mask<<(p->desc.alpha*(o%i)));
        p->alpha[o/i] |= (alpha&mask)<<(p->desc.alpha*(o%i));
        break;

    default:
        d_error_push("d_image_plot: unsupport alpha (report this as a bug).");
        return;
    }

    return;
}

void d_image_wipe(d_image_t *p, d_color_t c, byte alpha)
{
    int i;

    switch(p->desc.bpp) {
    case 8:
        d_memory_set(p->data, c, p->desc.h*p->desc.w);
        break;
        
    /* FIXME: I'm unsure about packing order here and below */
    case 15: /* fall thru */
    case 16:
        for(i = p->desc.w*p->desc.h*2; i > 0;) {
            p->data[--i] = c>>8;
            p->data[--i] = c;
        }
        break;

    case 24:
        for(i = p->desc.w*p->desc.h*3; i > 0;) {
            p->data[--i] = c>>16;
            p->data[--i] = c>>8;
            p->data[--i] = c;
        }
        break;

    case 32:
        for(i = p->desc.w*p->desc.h*4; i > 0;) {
            p->data[--i] = c>>16;
            p->data[--i] = c>>8;
            p->data[--i] = c;
            i--;
        }
        break;


    default:
        d_error_push("d_image_wipe: unsupported bpp (report this as a bug).");
        return;
    }

    switch(p->desc.alpha) {
    case 0:
        break;

    case 1:
    case 2:
    case 4:
    case 8:
        d_memory_set(p->alpha, alpha,
                     (p->desc.h*p->desc.w+(8/p->desc.alpha-1))/
                     (8/p->desc.alpha));
        break;

    default:
        d_error_push("d_image_wipe: unsupported alpha (report this as a bug).");
        return;
    }

    return;
}

void d_image_silhouette(d_image_t *p, d_color_t color, byte alpha)
{
    int i, o;
    byte mask;
    d_point_t pt;

    if(p->desc.alpha > 0) {
        mask = (1<<p->desc.alpha)-1;
        o = 8/p->desc.alpha;

        for(i = 0; i < p->desc.w*p->desc.h; i++) {
            if(p->alpha[i/o]&(mask<<(p->desc.alpha*(i%o)))) {
                pt.x = i%p->desc.w;
                pt.y = i/p->desc.w;
                d_image_plot(p, pt, color, alpha);
            }
        }
    } else {
        d_image_wipe(p, color, alpha);
    }
    return;
}

bool d_image_extendalpha(d_image_t *p, byte alpha)
{
    int i;
    byte *newalpha, t;

    if(alpha > 8) {
        d_error_push("d_image_extendalpha: desired alpha is too high.");
        return failure;
    }

    if(alpha == 0) { /* destroy alpha */
        if(p->alpha) d_memory_delete(p->alpha);
        p->desc.alpha = alpha;

    } else if(p->desc.alpha == 0) { /* create new opaque alpha */
        p->alpha = d_memory_new((p->desc.h*p->desc.w+(8/alpha-1))/(8/alpha));
        if(p->alpha == NULL)
            return failure;
        d_memory_set(p->alpha, 0xFF,
                     (p->desc.h*p->desc.w+(8/alpha-1))/(8/alpha));
        p->desc.alpha = alpha;

    } else { /* transform alpha */
        newalpha = d_memory_new((p->desc.h*p->desc.w+(8/alpha-1))/(8/alpha));
        if(newalpha == NULL)
            return failure;
        d_memory_set(newalpha, 0,
                     (p->desc.h*p->desc.w+(8/alpha-1))/(8/alpha));

        for(i = 0; i < p->desc.w*p->desc.h; i++) {
            /* FIXME this approximation could be better */
            t = p->alpha[i/(8/p->desc.alpha)]&((1<<p->desc.alpha)-1);

            if(alpha > p->desc.alpha) {
                newalpha[i/(8/alpha)] |= (t<<(alpha-p->desc.alpha))
                                           <<(i%(8/alpha));
            } else {
                newalpha[i/(8/alpha)] |= (t>>(p->desc.alpha-alpha))
                                           <<(i%(8/alpha));
            }

            d_memory_delete(p->alpha);
            p->alpha = newalpha;
            p->desc.alpha = alpha;
        }
    }

    return success;
}

bool d_image_convertdepth(d_image_t *p, byte bpp)
{
    byte *newdat, g;
    int i;

    if(p->desc.bpp == bpp) return success;
    if(bpp < 1 || bpp > 32) return failure;
    
    switch(p->desc.bpp) {
    case 1:
        switch(bpp) {
        case 16:
            newdat = d_memory_new(p->desc.w*p->desc.h*2);
            if(newdat == NULL) return failure;
            for(i = 0; i < p->desc.w*p->desc.h; i++) {
                if(p->data[i/8]&(1<<(i%8))) {
                    newdat[2*i+0] = 0xFF;
                    newdat[2*i+1] = 0xFF;
                } else {
                    newdat[2*i+0] = 0x00;
                    newdat[2*i+1] = 0x00;
                }
            }
            d_memory_delete(p->data);
            p->data = newdat;
            p->desc.bpp = bpp;
            break;

        case 24:
            newdat = d_memory_new(p->desc.w*p->desc.h*2);
            if(newdat == NULL) return failure;
            for(i = 0; i < p->desc.w*p->desc.h; i++) {
                if(p->data[i/8]&(1<<(i%8))) {
                    newdat[3*i+0] = 0xFF;
                    newdat[3*i+1] = 0xFF;
                    newdat[3*i+2] = 0xFF;
                } else {
                    newdat[3*i+0] = 0x00;
                    newdat[3*i+1] = 0x00;
                    newdat[3*i+2] = 0x00;
                }
            }
            d_memory_delete(p->data);
            p->data = newdat;
            p->desc.bpp = bpp;
            break;
        }
        break;

    case 8:
        switch(bpp) {
        case 16:
            newdat = d_memory_new(p->desc.w*p->desc.h*2);
            if(newdat == NULL) return failure;
            for(i = 0; i < p->desc.w*p->desc.h; i++) {
                g = p->palette.clut[3*p->data[i]+1]>>2;
                newdat[2*i+0] = (p->palette.clut[3*p->data[i]+2]>>3)|(g<<5);
                newdat[2*i+1] = ((p->palette.clut[3*p->data[i]+0]>>3)<<3)|
                    (g>>3);
            }
            d_memory_delete(p->data);
            p->data = newdat;
            p->desc.bpp = bpp;
            p->desc.paletted = false;
            break;
            
        case 24:
            newdat = d_memory_new(p->desc.w*p->desc.h*3);
            if(newdat == NULL) return failure;
            for(i = 0; i < p->desc.w*p->desc.h; i++) {
                newdat[3*i+0] = p->palette.clut[3*p->data[i]+0];
                newdat[3*i+1] = p->palette.clut[3*p->data[i]+1];
                newdat[3*i+2] = p->palette.clut[3*p->data[i]+2];
            }
            d_memory_delete(p->data);
            p->data = newdat;
            p->desc.bpp = bpp;
            p->desc.paletted = false;
            break;
        }
        break;

    case 24:
        switch(bpp) {
        case 16:
            newdat = d_memory_new(p->desc.w*p->desc.h*2);
            if(newdat == NULL) return failure;
            for(i = 0; i < p->desc.w*p->desc.h; i++) {
                g = p->data[3*i+1]>>2;
                newdat[2*i+0] = (p->data[3*i+2]>>3)|(g<<5);
                newdat[2*i+1] = ((p->data[3*i+0]>>3)<<3)|(g>>3);
            }
            d_memory_delete(p->data);
            p->data = newdat;
            p->desc.bpp = bpp;
            break;
        }
        break;

    default:
        d_error_push("d_image_convertdepth: Unsupported depth.");
        return failure;
    }
    return success;
}

/* EOF image.c */
