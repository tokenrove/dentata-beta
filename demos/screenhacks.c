/* 
 * screenhacks.c
 * Created: Tue Apr  3 16:20:06 2001 by tek@wiw.org
 * Revised: Wed Apr 11 07:33:59 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/raster.h>
#include <dentata/event.h>
#include <dentata/image.h>
#include <dentata/error.h>
#include <dentata/color.h>
#include <dentata/primitives.h>
#include <dentata/memory.h>
#include <dentata/time.h>

#define EV_QUIT 1
#define EV_CHANGECOLOR 2

d_image_t *init(void);
void munch(d_image_t *raster, int t);
void munchline(d_image_t *raster, int t, d_color_t c);
void shutdown(d_image_t *raster);

int main(int argc, char **argv)
{
    d_image_t *raster;

    raster = init();
    if(raster == NULL)
        d_error_fatal("init failed");

    munch(raster, 0);

    shutdown(raster);
    return 0;
}

#define NCOLORS 3

void munch(d_image_t *raster, int t)
{
    d_color_t c;
    void *qh;
    int i;
    d_color_t colors[NCOLORS] = { 0x10EE10, 0xEE1010, 0x1010EE };
    d_image_t *bg;
    d_point_t pt = { 0, 0 };

    bg = d_image_dup(raster);
    i = 0;
    c = d_color_frompackedrgb(bg, colors[i]);
    while(1) {
        d_event_update();
        if(d_event_ispressed(EV_QUIT))
            break;
        if(d_event_ispressed(EV_CHANGECOLOR))
            c = d_color_frompackedrgb(bg, colors[i]);
        qh = d_time_startcount(30);
        munchline(bg, t++, c);
        munchline(bg, t++, c);
        if(t%16 == 0) {
            c = d_color_frompackedrgb(bg, colors[i]);
            i = (i+1)%NCOLORS;
        }

        d_blit(raster, bg, pt);
        d_raster_update();
        d_time_endcount(qh);
    }
}

void munchline(d_image_t *raster, int t, d_color_t c)
{
    d_point_t pt;

    for(pt.x = 0; pt.x < raster->desc.w; pt.x++) {
        pt.y = pt.x ^ t;
        pt.x %= raster->desc.w;
        pt.y %= raster->desc.h;
        d_image_plot(raster, pt, c);
    }
    return;
}

d_image_t *init(void)
{
    d_image_t *raster;
    d_rasterdescription_t *modes;
    int nmodes, i;

    if(d_raster_new() == failure)
        return NULL;

    modes = d_raster_getmodes(&nmodes);

    for(i = 0; i < nmodes; i++) {
        if(modes[i].bpp < 8) continue;
        if(d_raster_setmode(modes[i]) == success) break;
    }

    if(i == nmodes)
        return NULL;

    raster = d_image_new(modes[i]);
    d_memory_delete(raster->data);
    raster->data = d_raster_getgfxpointer();
    d_raster_getpalette(&raster->palette);

    d_event_new(D_EVENT_KEYBOARD);

    d_event_map(EV_QUIT, D_KBD_ESCAPE);
    d_event_map(EV_CHANGECOLOR, D_KBD_TAB);

    return raster;
}

void shutdown(d_image_t *raster)
{
    d_event_delete();

    d_raster_delete();
    raster->data = NULL;
    d_image_delete(raster);

    return;
}

/* EOF screenhacks.c */
