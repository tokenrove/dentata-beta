/* 
 * s3mgen.c
 * Created: Sun Apr 15 16:06:01 2001 by tek@wiw.org
 * Revised: Mon Jul 16 18:59:55 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/sample.h>
#include <dentata/audio.h>
#include <dentata/s3m.h>
#include <dentata/file.h>
#include <dentata/memory.h>
#include <dentata/error.h>
#include <dentata/time.h>
#include <dentata/image.h>

#include "internal.h"

d_s3m_t *d_s3m_load(char *filename);
void d_s3m_delete(d_s3m_t *s3m);
d_s3mhandle_t *d_s3m_play(d_s3m_t *s3m);
void d_s3m_pause(d_s3mhandle_t *);
void d_s3m_resume(d_s3mhandle_t *);
void d_d3m_stop(d_s3mhandle_t *);
bool d_s3m_update(d_s3mhandle_t *);
static bool readinstrument(d_file_t *fp, d_s3m_instrument_t *inst);
static bool readpattern(d_file_t *fp, d_s3m_pattern_t *pat);

/*
 * loading routines
 */

d_s3m_t *d_s3m_load(char *filename)
{
    d_file_t *fp;
    int i, para, pos;
    d_s3m_t *s3m;

    fp = d_file_open(filename);
    if(fp == NULL)
        return NULL;

    s3m = d_memory_new(sizeof(d_s3m_t));
    if(s3m == NULL)
        return NULL;

    d_file_read(fp, s3m->name, S3M_TITLELEN);

    if(d_file_getbyte(fp) != 0x1A ||
       d_file_getbyte(fp) != 0x10) /* bad type */
        return NULL;

    d_file_seek(fp, 2, current); /* reserved bytes */
    s3m->norders = d_file_getword(fp);
    s3m->ninstruments = d_file_getword(fp);
    s3m->npatterns = d_file_getword(fp);

    s3m->orders = d_memory_new(s3m->norders);
    s3m->instruments = d_memory_new(s3m->ninstruments*
                                    sizeof(d_s3m_instrument_t));
    s3m->patterns = d_memory_new(s3m->npatterns*sizeof(d_s3m_pattern_t));

    if(s3m->orders == NULL ||
       s3m->instruments == NULL ||
       s3m->patterns == NULL)
        return NULL;

    s3m->flags = d_file_getword(fp);
    d_file_seek(fp, 2, current); /* created with tracker <blah> */
    s3m->ffi = d_file_getword(fp);

    if(d_file_getbyte(fp) != 'S' ||
       d_file_getbyte(fp) != 'C' ||
       d_file_getbyte(fp) != 'R' ||
       d_file_getbyte(fp) != 'M') /* bad magic */
        return NULL;

    s3m->gvolume = d_file_getbyte(fp);
    s3m->speed = d_file_getbyte(fp);
    s3m->tempo = d_file_getbyte(fp);
    s3m->mvolume = d_file_getbyte(fp);
    s3m->ultraclick = d_file_getbyte(fp);
    s3m->usedefaultpans = d_file_getbyte(fp);
    d_file_seek(fp, 10, current); /* reserved bytes, special word */

    d_file_read(fp, s3m->channelset, S3M_NCHANNELS);
    d_file_read(fp, s3m->orders, s3m->norders);

    for(i = 0; i < s3m->ninstruments; i++) {
        para = d_file_getword(fp)<<4;
        pos = d_file_tell(fp);
        d_file_seek(fp, para, beginning);
        if(readinstrument(fp, &s3m->instruments[i]) != success)
            return NULL;
        d_file_seek(fp, pos, beginning);
    }
    for(i = 0; i < s3m->npatterns; i++) {
        para = d_file_getword(fp)<<4;
        pos = d_file_tell(fp);
        d_file_seek(fp, para, beginning);
        if(readpattern(fp, &s3m->patterns[i]) != success)
            return NULL;
        d_file_seek(fp, pos, beginning);
    }

    if(s3m->usedefaultpans == 252)
        d_file_read(fp, s3m->channelpan, S3M_NCHANNELS);

    d_file_close(fp);
    return s3m;
}

bool readinstrument(d_file_t *fp, d_s3m_instrument_t *inst)
{
    dword memseg;
    d_audiomode_t mode;

    mode.frequency = 0;
    mode.bitspersample = 8;
    mode.nchannels = 1;
    mode.encoding = PCM;

    inst->type = d_file_getbyte(fp);
    d_file_read(fp, inst->filename, S3M_FILENAMELEN);

    /* memseg is a special case because it's 3 bytes, but this is
     * really messed up (FIXED: see below). seems that only the last two bytes
     * are used. WTF?
     *
     * FIXED: This is an error in the S3M document used as a reference. */

    d_file_seek(fp, 1, current);
    memseg = d_file_getword(fp);
    memseg *= 16;

    if(inst->type == sample) {
        inst->sample = d_sample_new(mode);
        inst->sample->len = d_file_getdword(fp);
        inst->sample->lbegin = d_file_getdword(fp);
        inst->sample->lend = d_file_getdword(fp);
        inst->volume = d_file_getbyte(fp);
        d_file_seek(fp, 1, current); /* reserved byte */
        inst->pack = d_file_getbyte(fp);
        inst->flags = d_file_getbyte(fp);
        if(inst->flags&1) inst->sample->hasloop = true;
        if(inst->flags&2) {
            d_error_fatal(__FUNCTION__": stereo sample (%s)\n", inst->name);
            inst->sample->mode.nchannels = 2;
            inst->sample->len *= 2;
        }
        if(inst->flags&4) {
            inst->sample->mode.bitspersample = 16;
        }
    } else {
        d_file_read(fp, inst->adlibspecs, S3M_ADLIBSPECLEN);
        inst->volume = d_file_getbyte(fp);
        d_file_seek(fp, 1, current); /* FIXME wtf is Dsk? */
        d_file_seek(fp, 2, current); /* reserved bytes */
    }

    inst->c2speed = d_file_getdword(fp);
    d_file_seek(fp, 12, current); /* reserved and internal bytes */
    d_file_read(fp, inst->name, S3M_INSTNAMELEN);

    if(inst->type == sample) {
        inst->sample->mode.frequency = inst->c2speed;
        if(d_file_getbyte(fp) != 'S' ||
           d_file_getbyte(fp) != 'C' ||
           d_file_getbyte(fp) != 'R' ||
           d_file_getbyte(fp) != 'S') /* bad magic */
            return failure;
    } else if(inst->type != 0) {
        if(d_file_getbyte(fp) != 'S' ||
           d_file_getbyte(fp) != 'C' ||
           d_file_getbyte(fp) != 'R' ||
           d_file_getbyte(fp) != 'I') /* bad magic */
            return failure;
    }

    if(inst->type == sample) {
        inst->sample->data = d_memory_new(inst->sample->len);
        if(inst->sample->data == NULL && inst->sample->len)
            return failure;

        /* we can seek like this because we'll get reseeked after we return */
        d_file_seek(fp, memseg, beginning);
        d_file_read(fp, inst->sample->data, inst->sample->len);
    }

    return success;
}

bool readpattern(d_file_t *fp, d_s3m_pattern_t *pat)
{
    int row, length, i, chan;

    d_memory_set(pat, 255, sizeof(d_s3m_pattern_t));

    row = 0;
    length = d_file_getword(fp);
    while(length > 0 && row < S3M_NROWS) {
        i = d_file_getbyte(fp); length--;
        if(i == 0) {
            row++;
        }

        chan = i&31;

        if(i&32) {
            pat->note[row][chan] = d_file_getbyte(fp); length--;
            pat->instrument[row][chan] = d_file_getbyte(fp); length--;
        }

        if(i&64) {
            pat->volume[row][chan] = d_file_getbyte(fp); length--;
        }

        if(i&128) {
            pat->command[row][chan] = d_file_getbyte(fp); length--;
            pat->operand[row][chan] = d_file_getbyte(fp); length--;
        }
    }

    return 1;
}

void d_s3m_delete(d_s3m_t *s3m)
{
    int i;

    d_memory_delete(s3m->orders);
    d_memory_delete(s3m->patterns);
    for(i = 0; i < s3m->ninstruments; i++) {
        if(s3m->instruments[i].type == sample)
            d_sample_delete(s3m->instruments[i].sample);
    }
    d_memory_delete(s3m->instruments);
    d_memory_delete(s3m);
    return;
}


/*
 * playback routines
 */


typedef struct s3mplayback_s {
    dword magic;
    word curorder, currow, nextorder, nextrow, counter;
    word speed, tempo, gvolume;
    byte patterndelay;
    int finevslide[S3M_NCHANNELS], finepitch[S3M_NCHANNELS];
    d_s3m_t *cursong;
    void *qh;
    bool playing, dobreak;
    d_s3m_instrument_t *lastinst[S3M_NCHANNELS];
} s3mplayback_t;

/* From FC's S3M tech doc */
static dword periods[12*2] = {
    1712*16,1616*16,1524*16,1440*16,1356*16,1280*16,1208*16,1140*16,
    1076*16,1016*16,960*16,907*16
};

#define MAXVOLUME 64

static void updatechannel(s3mplayback_t *pb, byte chan);
static void updatechanneltick(s3mplayback_t *pb, byte chan);
static void playatpitch(byte chan, d_s3m_instrument_t *inst, byte note);

d_s3mhandle_t *d_s3m_play(d_s3m_t *s3m)
{
    int i;
    d_channelprops_t props;
    s3mplayback_t *pb;
    bool status;

    props.mute = false;
    props.volume = 64;
    props.panning = 0;

    status = success;
    if(d_audio_nchannels() < S3M_NCHANNELS)
        for(i = d_audio_nchannels(); i < S3M_NCHANNELS; i++)
            status &= d_audio_addchannel(props);
    if(status != success) {
        d_error_push(__FUNCTION__": Unable to allocate sufficient audio "
                     "channels.");
        return NULL;
    }

    pb = d_memory_new(sizeof(s3mplayback_t));
    if(pb == NULL) {
        d_error_push("d_s3m_play: Unable to allocate internal data.");
        return NULL;
    }
    d_memory_set(pb, 0, sizeof(s3mplayback_t));
    pb->magic = S3MMAGIC;

    for(i = 0; i < S3M_NCHANNELS; i++) {
        pb->lastinst[i] = NULL;
        pb->finevslide[i] = 0;
    }
    pb->curorder = 0;
    pb->currow = 0;
    pb->cursong = s3m;
    pb->playing = true;
    pb->speed = s3m->speed;
    pb->tempo = s3m->tempo;
    pb->gvolume = s3m->gvolume;
    pb->counter = pb->speed;
    pb->patterndelay = 0;
    pb->dobreak = false;
    pb->qh = d_time_startcount(2*pb->tempo/5, false);

    return pb;
}

void d_s3m_pause(d_s3mhandle_t *p_)
{
    s3mplayback_t *p = (s3mplayback_t *)p_;
    if(p->magic != S3MMAGIC)
        d_error_fatal(__FUNCTION__": Internal magic doesn't match!");
    p->playing = false;
    return;
}

void d_s3m_resume(d_s3mhandle_t *p_)
{
    s3mplayback_t *p = (s3mplayback_t *)p_;
    if(p->magic != S3MMAGIC)
        d_error_fatal(__FUNCTION__": Internal magic doesn't match!");
    if(p->cursong != NULL)
        p->playing = true;
    return;
}

void d_s3m_stop(d_s3mhandle_t *p_)
{
    s3mplayback_t *p = (s3mplayback_t *)p_;

    if(p->magic != S3MMAGIC)
        d_error_fatal(__FUNCTION__": Internal magic doesn't match!");
    d_memory_delete(p);
    return;
}

bool d_s3m_update(d_s3mhandle_t *pb_)
{
    s3mplayback_t *pb = (s3mplayback_t *)pb_;
    byte chan;

    if(pb->magic != S3MMAGIC)
        d_error_fatal(__FUNCTION__": Internal magic doesn't match!");

    if(pb->cursong->orders[pb->curorder] == 255)
        return true;

    /* No point going ahead and messing up the playback if it's
     * too early */
    if(d_time_iscountfinished(pb->qh) != true)
        return false;
    d_time_endcount(pb->qh);

    pb->counter++;
    if(pb->counter >= pb->speed) {
        if(pb->dobreak) {
            pb->curorder = pb->nextorder;
            pb->currow = pb->nextrow;
            pb->dobreak = false;
        }

        /* FIXED [should sanity check order and row here] */
        /* If we're at (or past) the end, loop to the beginning of the track */
        if(pb->curorder >= pb->cursong->norders ||
           pb->cursong->orders[pb->curorder] > pb->cursong->npatterns ||
           pb->currow > S3M_NROWS) {
            return true;
        }

        for(chan = 0; chan < S3M_NCHANNELS; chan++)
            updatechannel(pb, chan);

        if(pb->patterndelay != 0)
            pb->patterndelay--;

        if(pb->patterndelay == 0)
            pb->currow++;

        if(pb->currow >= S3M_NROWS) {
            pb->currow = 0;
            pb->curorder++;
        }
        pb->counter = 0;
    } else {
        for(chan = 0; chan < S3M_NCHANNELS; chan++)
            updatechanneltick(pb, chan);
    }

    pb->qh = d_time_startcount(2*pb->tempo/5, false);
    return false;
}

void updatechannel(s3mplayback_t *pb, byte chan)
{
    d_s3m_pattern_t *p;
    d_channelprops_t props;
    dword freq;
    byte inst, basepitch;

    d_audio_getchanprops(chan, &props);
    p = &pb->cursong->patterns[pb->cursong->orders[pb->curorder]];

    if(p->volume[pb->currow][chan] != 255) {
        if(p->volume[pb->currow][chan] > MAXVOLUME)
            p->volume[pb->currow][chan] = MAXVOLUME;
        
        props.volume = (pb->gvolume*p->volume[pb->currow][chan])>>6;
    }

    if(pb->finevslide[chan] != 0) {
        props.volume += pb->finevslide[chan];
        pb->finevslide[chan] = 0;
    }

    inst = p->instrument[pb->currow][chan];
    if(inst != 0 && inst != 255)
        pb->lastinst[chan] = &pb->cursong->instruments[inst-1];

    if(pb->finepitch[chan] != 0) {
        /* FIXME, this is probably broken. no care was taken determining
           what is the correct note or instrument here. */
        playatpitch(chan, pb->lastinst[chan],
                    p->note[pb->currow][chan]+pb->finepitch[chan]);
        pb->finepitch[chan] = 0;
    }
    
    if(p->note[pb->currow][chan] != 255 && pb->lastinst[chan] != NULL) {
        if(p->note[pb->currow][chan] == 254) {
            /* note end */
            d_audio_stopsample(chan);
        } else if(inst == 0) {
            /* reset volume */
            props.volume = p->volume[pb->currow][chan];
            d_error_debug("props.volume = %d\n", p->volume[pb->currow][chan]);
        } else {
            d_audio_stopsample(chan);
            playatpitch(chan, pb->lastinst[chan], p->note[pb->currow][chan]);
        }
    }
    
    switch(p->command[pb->currow][chan]) {
    case 0x01: /* Axx -- set speed */
        if(p->operand[pb->currow][chan] < 0x20) {
            pb->speed = p->operand[pb->currow][chan];
        } else {
            d_error_debug(__FUNCTION__": A%2x\n",p->operand[pb->currow][chan]);
        }
        break;

    case 0x02: /* Bxx -- jump to order xx */
        pb->dobreak = true;
        pb->nextorder = p->operand[pb->currow][chan];
        pb->nextrow = 0;
        break;

    case 0x03: /* Cxx -- break pattern to row xx */
        pb->dobreak = true;
        pb->nextorder = pb->curorder+1;
        pb->nextrow = p->operand[pb->currow][chan];
        break;

    case 0x04: /* Dxx -- volume slides */
        if((p->operand[pb->currow][chan]>>4) == 0) {
        } else if((p->operand[pb->currow][chan]&0xf) == 0) {
        } else if((p->operand[pb->currow][chan]>>4) == 0xf) {
            pb->finevslide[chan] = -(p->operand[pb->currow][chan]&0xf);
        } else if((p->operand[pb->currow][chan]&0xf) == 0xf) {
            pb->finevslide[chan] = (p->operand[pb->currow][chan])>>4;
        }
        break;

    case 0x05:
        if((p->operand[pb->currow][chan]>>4) == 0xf) {
            pb->finepitch[chan] = -p->operand[pb->currow][chan]&0xf;
        } else if((p->operand[pb->currow][chan]>>4) == 0xe) {
            d_error_debug(__FUNCTION__": E%2x\n",p->operand[pb->currow][chan]);
        }
        break;

    case 0x06:
        if((p->operand[pb->currow][chan]>>4) == 0xf) {
            pb->finepitch[chan] = p->operand[pb->currow][chan]&0xf;
        } else if((p->operand[pb->currow][chan]>>4) == 0xe) {
            d_error_debug(__FUNCTION__": F%2x\n",p->operand[pb->currow][chan]);
        }
        break;

    case 0x0F: /* Oxy -- set sample offset */
        d_audio_setsamplepos(chan, p->operand[pb->currow][chan]*256);
        break;

    case 0x13: /* Sxx (various) */
        switch(p->operand[pb->currow][chan]>>4) {
        case 0xE: /* pattern delay */
            if(pb->patterndelay == 0)
                pb->patterndelay = p->operand[pb->currow][chan]&0xf;
            break;

        default:
            d_error_debug(__FUNCTION__": S%2x\n",p->operand[pb->currow][chan]);
        }
        break;

    case 0x14: /* Txx -- set tempo to xx */
        pb->tempo = p->operand[pb->currow][chan];
        break;

    case 0x16: /* Vxx -- set global volume */
        d_error_debug(__FUNCTION__": V%2x\n",p->operand[pb->currow][chan]);
        if(p->operand[pb->currow][chan] <= MAXVOLUME)
            pb->gvolume = p->operand[pb->currow][chan];
        else {
            d_error_debug(__FUNCTION__": V%2x\n",p->operand[pb->currow][chan]);
        }
        break;

    case 0xFF: /* no effect */
        break;

    default:
        d_error_debug(__FUNCTION__": hit effect %x (%x)\n",
                      p->command[pb->currow][chan], p->operand[pb->currow][chan]);
    }

    if(props.volume > MAXVOLUME)
        props.volume = MAXVOLUME;
    
    d_audio_setchanprops(chan, props);
    return;
}


void updatechanneltick(s3mplayback_t *pb, byte chan)
{
    d_s3m_pattern_t *p;
    d_channelprops_t props;

    d_audio_getchanprops(chan, &props);
    p = &pb->cursong->patterns[pb->cursong->orders[pb->curorder]];

    switch(p->command[pb->currow][chan]) {
    case 0x04:
        if((p->operand[pb->currow][chan]>>4) == 0) {
            props.volume -= p->operand[pb->currow][chan]&0xf;
        } else if((p->operand[pb->currow][chan]&0xf) == 0) {
            props.volume += p->operand[pb->currow][chan]>>4;
        }
        break;

    case 0x05:
        /*
        if((p->operand[pb->currow][chan]>>4) <= 0xd) {
            if(pb->lastinst[chan] != NULL)
                playatpitch(chan, pb->lastinst[chan],
                            p->note[pb->currow][chan]-
                            p->operand[pb->currow][chan]);
                            } */
        break;

    case 0x06:
        /*
        if((p->operand[pb->currow][chan]>>4) <= 0xd) {
            if(pb->lastinst[chan] != NULL)
                playatpitch(chan, pb->lastinst[chan],
                            p->note[pb->currow][chan]+
                            p->operand[pb->currow][chan]);
                            } */
        break;

    case 0x07:
        break;
    }

    if(props.volume > MAXVOLUME)
        props.volume = MAXVOLUME;

    d_audio_setchanprops(chan, props);
    return;
}

void playatpitch(byte chan, d_s3m_instrument_t *inst, byte note)
{
    dword pos, freq;

    /* This set of code is somewhat explained in the S3M
     * tech doc, if you're curious. The magic numbers might
     * be carcinogenic, though. */
    freq = 8363L*periods[note&0x0f];
    freq >>= note>>4;
    freq /= inst->c2speed;
    freq = 14317456L/(freq?freq:1);

    d_audio_getsamplepos(chan, &pos);
    d_audio_stopsample(chan);
    d_audio_playsample(chan, inst->sample, freq);
    d_audio_setsamplepos(chan, pos);
}

/* EOF s3mgen.c */
