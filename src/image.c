/* 
 * image.c
 * Created: Sun Feb 25 01:57:37 2001 by tek@wiw.org
 * Revised: Sat May 19 12:29:07 2001 by tek@wiw.org
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
byte d_image_getpelalpha(d_image_t *p, d_point_t pt);
d_color_t d_image_getpelcolor(d_image_t *p, d_point_t pt);
void d_image_setpelalpha(d_image_t *p, d_point_t pt, byte alpha);
void d_image_setpelcolor(d_image_t *p, d_point_t pt, d_color_t c);

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
    d_image_setpelcolor(p, pt, c);
    d_image_setpelalpha(p, pt, alpha);
    return;
}

void d_image_wipe(d_image_t *p, d_color_t c, byte alpha)
{
    int i;
    d_point_t pt;
    byte mask, o;

    switch(p->desc.bpp) {
    case 16:
        for(i = 0; i < 2*p->desc.w*p->desc.h; i+=2) {
            p->data[i] = c;
            p->data[i+1] = c>>8;
        }
        break;

    default:
        for(pt.y = 0; pt.y < p->desc.h; pt.y++)
            for(pt.x = 0; pt.x < p->desc.w; pt.x++)
                d_image_setpelcolor(p, pt, c);
        break;
    }

    switch(p->desc.alpha) {
    case 0:
        break;
 
    case 1:
    case 2:
    case 4:
        mask = (1<<p->desc.alpha)-1;
        alpha >>= (8-p->desc.alpha);
        alpha &= mask;
        o = 8/p->desc.alpha;
        mask = 0;
        for(i = 0; i < o; i++) {
            mask |= alpha<<(p->desc.alpha*(i%o));
        }
        d_memory_set(p->alpha, mask, (p->desc.w*p->desc.h+o-1)/o);
        break;

    case 8:
        d_memory_set(p->alpha, alpha, p->desc.w*p->desc.h);
        break;

    default:
        for(pt.y = 0; pt.y < p->desc.h; pt.y++)
            for(pt.x = 0; pt.x < p->desc.w; pt.x++)
                d_image_setpelalpha(p, pt, alpha);
        break;
    }
    return;
}

void d_image_silhouette(d_image_t *p, d_color_t color, byte alpha)
{
    d_point_t pt;

    if(p->desc.alpha > 0) {
        for(pt.y = 0; pt.y < p->desc.h; pt.y++)
            for(pt.x = 0; pt.x < p->desc.h; pt.x++) {
                if(d_image_getpelalpha(p, pt))
                    d_image_plot(p, pt, color, alpha);
            }
    } else {
        d_image_wipe(p, color, alpha);
    }
    return;
}

bool d_image_extendalpha(d_image_t *p, byte alpha)
{
    int i;
    byte *newalpha, t, o;

    if(alpha > 8) {
        d_error_push("d_image_extendalpha: desired alpha is too high.");
        return failure;
    }

    if(alpha == 0) { /* destroy alpha */
        if(p->alpha) d_memory_delete(p->alpha);
        p->alpha = NULL;
        p->desc.alpha = alpha;

    } else if(p->desc.alpha == 0) { /* create new opaque alpha */
        p->alpha = d_memory_new((p->desc.h*p->desc.w+(8/alpha-1))/(8/alpha));
        if(p->alpha == NULL)
            return failure;
        d_memory_set(p->alpha, 0xFF,
                     (p->desc.h*p->desc.w+(8/alpha-1))/(8/alpha));
        p->desc.alpha = alpha;

    } else { /* transform alpha */
        o = 8/alpha;

        newalpha = d_memory_new((p->desc.h*p->desc.w+o-1)/o);
        if(newalpha == NULL)
            return failure;
        d_memory_set(newalpha, 0, (p->desc.h*p->desc.w+o-1)/o);

        for(i = 0; i < p->desc.w*p->desc.h; i++) {
            /* FIXME this approximation could be better */
            t = p->alpha[i/(8/p->desc.alpha)];
            t >>= p->desc.alpha*(i%(8/p->desc.alpha));
            t &= (word)(1<<p->desc.alpha)-1;

            if(alpha > p->desc.alpha) {
                newalpha[i/o] |= (t<<(alpha-p->desc.alpha))<<(alpha*(i%o));
            } else {
                newalpha[i/o] |= (t>>(p->desc.alpha-alpha))<<(alpha*(i%o));
            }
        }

        d_memory_delete(p->alpha);
        p->alpha = newalpha;
        p->desc.alpha = alpha;
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

void d_image_setpelcolor(d_image_t *p, d_point_t pt, d_color_t c)
{
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
        p->data[(pt.x+pt.y*p->desc.w)*2+0] = c&0xFF;
        p->data[(pt.x+pt.y*p->desc.w)*2+1] = c>>8;
        break;

    case 24:
        p->data[(pt.x+pt.y*p->desc.w)*3+0] = c&0xFF;
        p->data[(pt.x+pt.y*p->desc.w)*3+1] = (c>>8)&0xFF;
        p->data[(pt.x+pt.y*p->desc.w)*3+2] = (c>>16)&0xFF;
        break;

    case 32:
        p->data[(pt.x+pt.y*p->desc.w)*4+0] = c&0xFF;
        p->data[(pt.x+pt.y*p->desc.w)*4+1] = (c>>8)&0xFF;
        p->data[(pt.x+pt.y*p->desc.w)*4+2] = (c>>16)&0xFF;
        break;

    default:
        d_error_push("d_image_setpelcolor: unsupport bpp (report this as a bug).");
        return;
    }
    return;
}

void d_image_setpelalpha(d_image_t *p, d_point_t pt, byte alpha)
{
    byte mask, o;
    int i;

    switch(p->desc.alpha) {
    case 0:
        break;

    case 1:
    case 2:
    case 4:
    case 8:
        mask = (1<<p->desc.alpha)-1;
        alpha >>= (8-p->desc.alpha);
        i = pt.x+pt.y*p->desc.w;
        o = 8/p->desc.alpha;
        p->alpha[i/o] &= ~(mask<<(p->desc.alpha*(i%o)));
        p->alpha[i/o] |= (alpha&mask)<<(p->desc.alpha*(i%o));
        break;

    default:
        d_error_push("d_image_setpelalpha: unsupported alpha (report this as a bug).");
        return;
    }
    return;
}

d_color_t d_image_getpelcolor(d_image_t *p, d_point_t pt)
{
    int i = pt.x+pt.y*p->desc.w;
    d_color_t c;

    if(i < 0 || i >= p->desc.w*p->desc.h)
        return 0;

    switch(p->desc.bpp) {
    case 8:
        c = p->data[i];
        break;
        
    /* FIXME: I'm unsure about packing order here and below */
    case 15: /* fall thru */
    case 16:
        i *= 2;
        c = p->data[i++];
        c |= p->data[i++]<<8;
        break;

    case 24:
        i *= 3;
        c = p->data[i++];
        c |= p->data[i++]<<8;
        c |= p->data[i++]<<16;
        break;

    case 32:
        i *= 4;
        c = p->data[i++];
        c |= p->data[i++]<<8;
        c |= p->data[i++]<<16;
        break;


    default:
        d_error_push("d_image_getpelcolor: unsupported bpp (report this as a bug).");
        return 0;
    }

    return c;
}

byte d_image_getpelalpha(d_image_t *p, d_point_t pt)
{
    byte a, mask, i;
    int o;

    o = pt.x+pt.y*p->desc.w;
    if(o < 0 || o >= p->desc.w*p->desc.h)
        return 0;

    switch(p->desc.alpha) {
    case 0:
        return 255;

    case 1:
    case 2:
    case 4:
    case 8:
        mask = (1<<p->desc.alpha)-1;
        i = 8/p->desc.alpha;
        a = p->alpha[o/i]>>(p->desc.alpha*(o%i));
        a &= mask;
        a <<= 8-p->desc.alpha;
        break;

    default:
        d_error_push("d_image_getpelalpha: unsupport alpha (report this as a bug).");
        return 0;
    }

    return a;
}

d_image_t *d_image_rotate(d_image_t *p, byte theta)
{
    d_image_t *q;
    d_rasterdescription_t desc;
    d_point_t pt, pt2;
    int s_theta, c_theta;
    int dw, dh;

    desc = p->desc;
    s_theta = 0;
    c_theta = 1;

    switch(theta) {
    case 0:
        break;

    case 64:
        desc.w = p->desc.h;
        desc.h = p->desc.w;
        s_theta = 1;
        c_theta = 0;
        break;

    case 192:
        desc.w = p->desc.h;
        desc.h = p->desc.w;
        s_theta = -1;
        c_theta = 0;
        break;

    default:
        /* FIXME */
        d_error_fatal("d_image_rotate: sorry, we don't handle complex rotation yet.\n");
    }
    q = d_image_new(desc);

    dw = q->desc.w/2;
    dh = q->desc.h/2;

    for(pt.y = 0; pt.y < desc.h; pt.y++) {
        for(pt.x = 0; pt.x < desc.w; pt.x++) {
            pt2.x = (pt.x-dw)*c_theta+(pt.y-dh)*s_theta+p->desc.w/2;
            pt2.y = (pt.x-dw)*(-s_theta)+(pt.y-dh)*c_theta+p->desc.h/2;
            /* FIXME hack */
            if(s_theta == -1) pt2.x--;
            if(s_theta == 1) pt2.y--;
            d_image_plot(q, pt, d_image_getpelcolor(p, pt2),
                         d_image_getpelalpha(p, pt2));
        }
    }

    return q;
}

/* EOF image.c */
