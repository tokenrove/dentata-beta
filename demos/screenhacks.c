/* 
 * screenhacks.c
 * Created: Tue Apr  3 16:20:06 2001 by tek@wiw.org
 * Revised: Sun Apr 15 16:09:07 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/image.h>
#include <dentata/blit.h>
#include <dentata/raster.h>
#include <dentata/event.h>
#include <dentata/error.h>
#include <dentata/color.h>
#include <dentata/memory.h>
#include <dentata/tga.h>
#include <dentata/time.h>
#include <dentata/font.h>
#include <dentata/sample.h>
#include <dentata/audio.h>
#include <dentata/wav.h>

#define EV_QUIT 1
#define FPS 72

d_image_t *init(void);
void munch(d_image_t *raster, int t);
void munchline(d_image_t *raster, int t, d_color_t c, byte alpha);
void shutdown(d_image_t *raster);

int main(int argc, char **argv)
{
    d_image_t *raster;

    raster = init();
    if(raster == NULL)
        d_error_fatal("init failed");

    munch(raster, 0);

    shutdown(raster);
    d_error_dump();
    return 0;
}

#define NCOLORS 3

void munch(d_image_t *raster, int t)
{
    void *qh;
    int i, j;
    d_color_t colors[NCOLORS] =   { 0x10EE10, 0xEE1010, 0x1010EE },
              fgcolors[NCOLORS] = { 0x000000, 0xFFFFFF, 0xFF0000 };
    d_image_t *bg, *fg, *sprite, *spsil;
    d_point_t bgpt = { 0, 0 }, fgpt = { 0, 0 }, sprpt = { 100, 100 },
                                                    sproldpt;
    byte fgalpha[NCOLORS] = { 0, 255, 128 }, spralpha;
    d_font_t *font;

    bg = d_image_dup(raster);
    fg = d_image_dup(bg);
    if(d_image_extendalpha(fg, 4) != success)
        return;
    d_image_wipe(fg, 0, 0);

    sprite = d_tga_load("sprite.tga");
    d_image_convertdepth(sprite, raster->desc.bpp);
    spsil = d_image_dup(sprite);
    d_image_silhouette(spsil, d_color_fromrgb(spsil, 0,0,0), 140);

    font = d_font_load("readable.fnt");
    if(font == NULL)
        return;

    i = 0; j = 0;
    spralpha = 255;

    while(1) {
        d_event_update();
        if(d_event_ispressed(EV_QUIT))
            break;

        qh = d_time_startcount(FPS);

        munchline(bg, t++, d_color_frompackedrgb(bg, colors[i]), 0xFF);
        munchline(bg, t++, d_color_frompackedrgb(bg, colors[i]), 0xFF);

        munchline(fg, t-2, d_color_frompackedrgb(fg, fgcolors[i]), fgalpha[i]);
        munchline(fg, t-1, d_color_frompackedrgb(fg, fgcolors[i]), fgalpha[i]);

        if(t%16 == 0) {
            i = (i+1)%NCOLORS;
            j++;
        }

        sproldpt = sprpt;

        if(t%64 < 31) {
            fgpt.x -= raster->desc.w/100;
            sprpt.x -= raster->desc.w/60;
        } else {
            fgpt.x += raster->desc.w/100;
            sprpt.x += raster->desc.w/60;
        }

        if(t%32 < 15) {
            fgpt.y -= raster->desc.h/100;
            sprpt.y -= raster->desc.h/60;
        } else {
            fgpt.y += raster->desc.h/100;
            sprpt.y += raster->desc.h/60;
        }

	if(t%64 < 31) {
            spralpha += 16;
	} else {
            spralpha -= 16;
	}

	if(spralpha > 0)
            for(j = 0; j < sprite->desc.w*sprite->desc.h; j++)
                if(sprite->alpha[j] > 0) sprite->alpha[j] = spralpha;

        d_blit(raster, bg, bgpt);
        d_blit(raster, spsil, sproldpt);
        d_blit(raster, sprite, sprpt);
        d_font_printf(raster, font, sprpt, "Foo! %d");
        d_blit(raster, fg, fgpt);

        d_raster_update();
        d_audio_update();
        d_time_endcount(qh);
    }

    d_font_delete(font);
    d_image_delete(bg);
    d_image_delete(fg);
    d_image_delete(sprite);
    d_image_delete(spsil);
}

void munchline(d_image_t *raster, int t, d_color_t c, byte alpha)
{
    d_point_t pt;

    for(pt.x = 0; pt.x < raster->desc.w; pt.x++) {
        pt.y = pt.x ^ t;
        pt.x %= raster->desc.w;
        pt.y %= raster->desc.h;
        d_image_plot(raster, pt, c, alpha);
    }
    return;
}

d_image_t *init(void)
{
    d_image_t *raster;
    d_rasterdescription_t *modes;
    d_sample_t *sample;
    d_channelprops_t props;
    int nmodes, i;

    if(d_raster_new() == failure)
        return NULL;

    modes = d_raster_getmodes(&nmodes);

    for(i = 0; i < nmodes; i++) {
        if(modes[i].bpp < 16) continue;
        if(d_raster_setmode(modes[i]) == success) break;
    }

    if(i == nmodes)
        return NULL;

    raster = d_image_new(modes[i]);
    d_memory_delete(modes);
    d_memory_delete(raster->data);
    raster->data = d_raster_getgfxpointer();
    d_raster_getpalette(&raster->palette);

    d_event_new(D_EVENT_KEYBOARD);

    d_event_map(EV_QUIT, D_KBD_ESCAPE);

    props.mute = false;
    props.volume = 255;
    props.panning = 0;
    sample = d_wav_load("foo.wav");
    if(sample != NULL) {
        d_audio_new(sample->mode);
        d_audio_addchannel(props);
        d_audio_playsample(0, sample, sample->mode.frequency);
    }

    return raster;
}

void shutdown(d_image_t *raster)
{
    d_audio_delete();
    raster->data = NULL;
    d_image_delete(raster);
    d_event_delete();
    d_raster_delete();
    return;
}

/* EOF screenhacks.c */
