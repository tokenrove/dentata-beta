/* 
 * pcx.c
 * Created: Sun Apr 15 17:15:31 2001 by tek@wiw.org
 * Revised: Sun Apr 15 17:28:41 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>
#include <dentata/pcx.h>
#include <dentata/memory.h>
#include <dentata/file.h>
#include <dentata/error.h>

struct pcxheader_s {
    /* manufacturer byte */
    char version;
    char encoding;
    char bps;
    short xmin, ymin, xmax, ymax;
    short hres, yres;
    char colormap[48];
    /* reserved byte */
    char nplanes;
    short bpsl;
    short palettetype;
    /* 58 bytes of filler */
};

d_image_t *d_pcx_load(char *fname);
static void decodepcxrle(d_file_t *, d_image_t *, int, int);
static struct pcxheader_s readpcxheader(d_file_t *);

d_image_t *d_pcx_load(char *fname)
{
    d_file_t *fp;
    d_image_t *p;
    struct pcxheader_s head;
    d_rasterdescription_t desc;

    fp = d_file_open(fname);
    if(fp == NULL) return NULL;

    head = readpcxheader(fp);
    if(head.version < 3)
        return NULL; /* we don't support versions older than 3 */
    if(head.encoding != 1)
        return NULL; /* encoding == 1 (RLE) */

    desc.w = head.xmax-head.xmin+1;
    desc.h = head.ymax-head.ymin+1;
    if(head.nplanes == 1)
        desc.bpp = 8;
    else if(head.nplanes >= 3)
        desc.bpp = 24;

    if(head.palettetype == 2)
        desc.cspace = grayscale;
    else
        desc.cspace = RGB;

    if(head.nplanes == 1 && head.palettetype == 1)
        desc.paletted = true;

    if(head.nplanes == 4)
        desc.alpha = 8;
    else
        desc.alpha = 1;

    p = d_image_new(desc);
    if(p == NULL)
        return NULL;

    decodepcxrle(fp, p, head.nplanes, head.bpsl);

    d_file_seek(fp, -(D_BYTESPERCOLOR*D_NCLUTITEMS+1), end);
    if(head.version == 5 && (p->desc.paletted) && d_file_getbyte(fp) == 12) {
        d_file_read(fp, p->palette.clut, D_BYTESPERCOLOR*D_NCLUTITEMS);
    }

    d_file_close(fp);
    return p;
}

void decodepcxrle(d_file_t *fp, d_image_t *p, int nplanes, int bpsl)
{
    int run, i, j, dy;
    unsigned char c;

    run = 0;
    for(i = 0; i < nplanes*p->desc.h; i++) {
        dy = (i/nplanes)*p->desc.w;
        for(j = 0; j < bpsl;) {
            c = d_file_getbyte(fp);
            if(run == 0) {
                if(c > 0xBF)
                    run = c&0x3F; /* 0x3F == ~0xC0 */
                else {
                    p->data[dy+nplanes*j+(i%nplanes)] = c;
                    j++;
                }
                continue;
            }
            for(; run > 0; run--, j++)
                p->data[dy+nplanes*j+(i%nplanes)] = c;
            run = 0;
        }
    }
    return;
}

struct pcxheader_s readpcxheader(d_file_t *fp)
{
    struct pcxheader_s p;

    d_file_getbyte(fp); /* manufacturer == 0 */
    p.version = d_file_getbyte(fp);
    p.encoding = d_file_getbyte(fp);
    p.bps = d_file_getbyte(fp);
    p.xmin = d_file_getword(fp);
    p.ymin = d_file_getword(fp);
    p.xmax = d_file_getword(fp);
    p.ymax = d_file_getword(fp);
    d_file_seek(fp, 53, current);
    p.nplanes = d_file_getbyte(fp);
    p.bpsl = d_file_getword(fp);
    p.palettetype = d_file_getword(fp);
    d_file_seek(fp, 58, current);

    return p;
}

/* EOF pcx.c */
