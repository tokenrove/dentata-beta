/* 
 * viewtga.c
 * Created: Wed Apr 11 07:08:12 2001 by tek@wiw.org
 * Revised: Thu Apr 19 05:24:49 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/raster.h>
#include <dentata/image.h>
#include <dentata/tga.h>
#include <dentata/error.h>
#include <dentata/event.h>
#include <dentata/time.h>
#include <dentata/memory.h>

#include <stdio.h>

#define PROGNAME "viewtga"
#define EV_QUIT 1

bool modehaggle(d_rasterdescription_t mode, d_image_t **raster,
                bool haggledimensions);

int main(int argc, char **argv)
{
    char *infile;
    d_image_t *raster, *tga;
    d_point_t pt = { 0, 0 };

    if(argc < 2)
        d_error_fatal("usage: %s <tga file>\n", PROGNAME);
    infile = argv[1];

    tga = d_tga_load(infile);
    if(tga == NULL)
        d_error_fatal("unable to load %s.\n", infile);

    if(d_raster_new() != success)
        d_error_fatal("unable to init raster.\n");
    if(modehaggle(tga->desc, &raster, true) != success &&
       modehaggle(tga->desc, &raster, false) != success) {
        d_error_fatal("unable to locate a suitable graphics mode.\n");
    }

    if(d_event_new(D_EVENT_KEYBOARD) != D_EVENT_KEYBOARD)
        d_error_fatal("unable to init keyboard.\n");
    d_event_map(EV_QUIT, D_KBD_ESCAPE);
    d_event_map(EV_QUIT, D_KBD_Q);

    d_image_blit(raster, tga, pt);
    d_raster_update();

    while(!d_event_ispressed(EV_QUIT)) {
        d_event_update();
        d_time_wait(500);
    }

    d_event_delete();
    d_raster_delete();
    raster->data = NULL;
    d_image_delete(raster);

    d_image_delete(tga);
    return 0;
}

bool modehaggle(d_rasterdescription_t mode, d_image_t **raster,
                bool haggledimensions)
{
    int i, nmodes;
    d_rasterdescription_t *modes;

    modes = d_raster_getmodes(&nmodes);

    for(i = 0; i < nmodes; i++) {
        if(modes[i].bpp > 8 &&
           haggledimensions&(modes[i].w >= mode.w) &&
           haggledimensions&(modes[i].h >= mode.h)) {
            if(d_raster_setmode(modes[i]) == success) {
                *raster = d_image_new(modes[i]);
                d_memory_delete((*raster)->data);
                (*raster)->data = d_raster_getgfxpointer();
                break;
            }
        }
    }

    d_memory_delete(modes);

    return (i < nmodes) ? success:failure;
}
/* EOF viewtga.c */
