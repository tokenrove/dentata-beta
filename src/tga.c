/* 
 * tga.c
 * Created: Sun Feb 25 02:56:14 2001 by tek@wiw.org
 * Revised: Sat May  5 10:22:32 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>
#include <dentata/tga.h>
#include <dentata/file.h>
#include <dentata/error.h>
#include <dentata/memory.h>

struct tgaheader_s {
    byte type, commentlen, encoding, cmaptype;
    word cmaporig, cmaplen;
    byte cmapbpp, bpp;
    d_rect_t rect;
};

d_image_t *d_tga_load(char *filename);
static void readnormaltga(d_file_t *fp, d_image_t *p);
static void readrletga(d_file_t *fp, d_image_t *p);
static struct tgaheader_s readtgaheader(d_file_t *fp);
static void readtgacmap(d_file_t *fp, d_palette_t *palette,
                        struct tgaheader_s head);

d_image_t *d_tga_load(char *filename)
{
    d_image_t *p;
    d_rasterdescription_t desc;
    struct tgaheader_s head;
    d_file_t *fp;

    fp = d_file_open(filename);
    if(fp == NULL) return NULL;

    head = readtgaheader(fp);
    if(head.cmaptype == 1)
        desc.paletted = true;
    else
        desc.paletted = false;
    desc.cspace = RGB;

    switch(head.bpp) {
    case 32:
        desc.bpp = 24;
        desc.alpha = 8;
        break;

    default:
        desc.bpp = head.bpp;
        desc.alpha = 0;
        break;
    }

    desc.w = head.rect.w;
    desc.h = head.rect.h;

    p = d_image_new(desc);
    if(desc.paletted)
        readtgacmap(fp, &p->palette, head);

    if(head.encoding == 0) {
        readnormaltga(fp, p);
    } else if(head.encoding == 1) {
        readrletga(fp, p);
    } else {
        d_error_push("d_tga_load: unsupported TGA subformat.");
        return NULL;
    }

    d_file_close(fp);
    return p;
}

void readnormaltga(d_file_t *fp, d_image_t *p)
{
    int i;

    i = (p->desc.bpp+7)/8;

    if(p->desc.alpha != 8)
        d_file_read(fp, p->data, p->desc.w*p->desc.h*i);
    else {
        for(i = 0; i < p->desc.w*p->desc.h; i++) {
            p->data[3*i+2] = d_file_getbyte(fp);
            p->data[3*i+1] = d_file_getbyte(fp);
            p->data[3*i+0] = d_file_getbyte(fp);
            p->alpha[i] = d_file_getbyte(fp);
        }
    }
    return;
}

void readrletga(d_file_t *fp, d_image_t *p)
{
    int i, j, k, run, inc;
    long c;

    inc = (p->desc.bpp+7)/8;

    /* FIXME: rearrange p->data read order or pack or similar? */
    for(i = 0; i < p->desc.w*p->desc.h;) {
        run = d_file_getbyte(fp);
        if(run&0x80) {
            run &= 0x7F;

            for(j = 0, c = 0; j < inc; j++)
                c |= d_file_getbyte(fp)<<(8*j);
            if(p->desc.alpha == 8)
                c |= d_file_getbyte(fp)<<(8*j);

            for(j = 0; j < run+1; j++, i++) {
                for(k = 0; k < inc; k++)
                    p->data[inc*i+(inc-k-1)] = c>>(8*k);
                if(p->desc.alpha == 8) p->alpha[i] = c>>(8*k);
            }
        } else {
            for(j = 0; j < run+1; j++, i++) {
                for(k = 0; k < inc; k++)
                    p->data[inc*i+(inc-k-1)] = d_file_getbyte(fp);
                if(p->desc.alpha == 8)
                    p->alpha[i] = d_file_getbyte(fp);
            }
        }
    }
    return;
}

void readtgacmap(d_file_t *fp, d_palette_t *cmap, struct tgaheader_s head)
{
    int i;

    for(i = head.cmaporig; i < head.cmaporig+head.cmaplen &&
            i < D_NCLUTITEMS; i++) {
        switch(head.cmapbpp) {
        case 16:
            d_color_unpack(head.cmapbpp, d_file_getword(fp),
                           &cmap->clut[3*i+0], &cmap->clut[3*i+1],
                           &cmap->clut[3*i+2]);
            break;
            
        case 24:
            cmap->clut[3*i+2] = d_file_getbyte(fp);
            cmap->clut[3*i+1] = d_file_getbyte(fp);
            cmap->clut[3*i+0] = d_file_getbyte(fp);
            break;

            /* FIXME might be a different order */
        case 32:
            cmap->clut[3*i+2] = d_file_getbyte(fp);
            cmap->clut[3*i+1] = d_file_getbyte(fp);
            cmap->clut[3*i+0] = d_file_getbyte(fp);
            d_file_seek(fp, 1, current);
            break;

        default:
            d_error_push("d_tga_load: Unsupported colormap depth.");
            break;
	}
    }
    return;
}

struct tgaheader_s readtgaheader(d_file_t *fp)
{
    struct tgaheader_s head;

    d_memory_set(&head, 0, sizeof(struct tgaheader_s));
    head.commentlen = d_file_getbyte(fp);

    head.cmaptype = d_file_getbyte(fp);
    head.type = d_file_getbyte(fp);
    if(head.type == 1 || head.type == 2)
        head.encoding = 0;
    else if(head.type == 9 || head.type == 10)
        head.encoding = 1;

    head.cmaporig = d_file_getword(fp);
    head.cmaplen = d_file_getword(fp);
    head.cmapbpp = d_file_getbyte(fp);

    head.rect.offset.x = d_file_getword(fp);
    head.rect.offset.y = d_file_getword(fp);
    head.rect.w = d_file_getword(fp);
    head.rect.h = d_file_getword(fp);

    head.bpp = d_file_getbyte(fp);
    d_file_seek(fp, 1, current); /* image descriptor byte */
    d_file_seek(fp, head.commentlen, current);

    return head;
}

/* EOF tga.c */
