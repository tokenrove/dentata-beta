/* 
 * font.c
 * Created: Fri Apr 13 20:38:07 2001 by tek@wiw.org
 * Revised: Sat Apr 14 16:09:52 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/font.h>
#include <dentata/file.h>
#include <dentata/memory.h>
#include <dentata/error.h>
#include <dentata/blit.h>
#include <dentata/image.h>

d_font_t *d_font_load(char *);
void d_font_printf(d_image_t *, d_font_t *, d_point_t, byte *, ...);
word d_font_gettextwidth(d_font_t *, byte *, ...);
void d_font_delete(d_font_t *);

static bool loadv0font(d_file_t *fp, d_font_t *fnt);

d_font_t *d_font_load(char *filename)
{
    d_font_t *fnt;
    d_file_t *fp;

    fnt = d_memory_new(sizeof(d_font_t));
    if(fnt == NULL) return NULL;

    fp = d_file_open(filename);
    if(fp == NULL) return NULL;

    if(d_file_getbyte(fp) != 'F' ||
       d_file_getbyte(fp) != 'N' ||
       d_file_getbyte(fp) != 'T') {
        d_error_push("d_font_load: bad magic.");
        return NULL;
    }

    switch(d_file_getbyte(fp)) {
    case 0: /* version 0 -- bitmapped font */
        if(loadv0font(fp, fnt) != success)
            return NULL;
        break;

    default:
        d_error_push("d_font_load: unsupported font version.");
        return NULL;
    }

    d_file_close(fp);
    return fnt;
}

bool loadv0font(d_file_t *fp, d_font_t *fnt)
{
    int i, j;
    byte k, l;

    fnt->desc.w = d_file_getword(fp);
    fnt->desc.h = d_file_getword(fp);
    fnt->desc.bpp = 1;
    fnt->desc.alpha = 1;
    fnt->desc.paletted = false;
    fnt->desc.cspace = RGB;
    fnt->start = d_file_getbyte(fp);
    fnt->nchars = d_file_getbyte(fp);
    fnt->chars = d_memory_new(sizeof(d_image_t *)*fnt->nchars);
    if(fnt->chars == NULL) return failure;

    for(i = 0; i < fnt->nchars; i++) {
        fnt->chars[i] = d_image_new(fnt->desc);
        if(fnt->chars[i] == NULL) return failure;
        d_file_read(fp, fnt->chars[i]->data, fnt->desc.h);
        for(j = 0; j < fnt->desc.h; j++) {
            for(k = 0, l = 0; k < 8; k++) {
                l |= ((fnt->chars[i]->data[j]&(1<<k))>>k)<<(7-k);
            }
            fnt->chars[i]->data[j] = l;
        }
        d_memory_copy(fnt->chars[i]->alpha, fnt->chars[i]->data,
                      fnt->desc.h);
    }

    return success;
}

void d_font_printf(d_image_t *image, d_font_t *fnt, d_point_t pt, byte *fmt,
                   ...)
{
    void *args;
    byte *p;
    int i, len;

    args = fmt+sizeof(byte *);
//        len = crash_getlen(fmt, args);
    for(len = 0; fmt[len]; len++);
    p = d_memory_new(len+1);
    for(i = 0; i < len; i++)
        p[i] = fmt[i];

    for(i = 0; i < len; i++) {
        d_blit(image, fnt->chars[(unsigned)p[i]], pt);
        pt.x += fnt->desc.w+1;
    }
    d_memory_delete(p);
    return;
}

word d_font_gettextwidth(d_font_t *fnt, byte *fmt, ...)
{
    return 0;
}

void d_font_delete(d_font_t *fnt)
{
    int i;

    for(i = 0; i < fnt->nchars; i++)
        d_image_delete(fnt->chars[i]);
    d_memory_delete(fnt->chars);
    d_memory_delete(fnt);
    return;
}

/* EOF font.c */