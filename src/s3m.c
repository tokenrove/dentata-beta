/* 
 * s3m.c
 * Created: Sun Apr 15 16:06:01 2001 by tek@wiw.org
 * Revised: Sat May 19 13:04:19 2001 by tek@wiw.org
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

d_s3m_t *d_s3m_load(char *filename);
void d_s3m_delete(d_s3m_t *s3m);
void d_s3m_pause(void);
void d_s3m_resume(void);
void d_d3m_stop(void);
void d_s3m_update(void);
static bool readinstrument(d_file_t *fp, d_s3m_instrument_t *inst);
static bool readpattern(d_file_t *fp, d_s3m_pattern_t *pat);

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
     * really messed up (FIXME). seems that only the last two bytes
     * are used. WTF? */
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

/* playback section */
/* FIXME -- excessive statics, prefer more threadsafe system of passing
   playback information with each call */
static word curorder, currow, counter, speed, tempo, gvolume;
static d_s3m_t *cursong;
static void *qh;
static bool playing = false;
static d_s3m_instrument_t *lastinst[S3M_NCHANNELS];
static dword periods[12*2] = {
    1712*16,1616*16,1524*16,1440*16,1356*16,1280*16,1208*16,1140*16,
    1076*16,1016*16,960*16,907*16
};

void d_s3m_play(d_s3m_t *s3m)
{
    int i;
    d_channelprops_t props;

    props.mute = false;
    props.volume = 192;
    props.panning = 0;

    if(d_audio_nchannels() < S3M_NCHANNELS) {
        for(i = d_audio_nchannels(); i < S3M_NCHANNELS; i++)
            d_audio_addchannel(props);
    }
    for(i = 0; i < S3M_NCHANNELS; i++)
        lastinst[i] = NULL;
    curorder = 0;
    currow = 0;
    cursong = s3m;
    playing = true;
    speed = s3m->speed;
    tempo = s3m->tempo;
    gvolume = s3m->gvolume;
    counter = speed;
    qh = d_time_startcount(2*tempo/5, false);
    return;
}

void d_s3m_pause(void)
{
    playing = false;
    return;
}

void d_s3m_resume(void)
{
    if(cursong != NULL)
        playing = true;
    return;
}

void d_s3m_stop(void)
{
    playing = false;
    cursong = NULL;
    return;
}

void d_s3m_update(void)
{
    d_s3m_pattern_t *p;
    d_channelprops_t props;
    int i;
    dword freq, note, octave;

    if(cursong == NULL) return;

    if(d_time_iscountfinished(qh) != true) return;
    d_time_endcount(qh);
    counter++;
    if(counter >= speed) {
        if(cursong->orders[curorder] > cursong->npatterns ||
           curorder >= cursong->norders) {
            curorder = 0;
            tempo = cursong->tempo;
            speed = cursong->speed;
        }
        p = &cursong->patterns[cursong->orders[curorder]];
        for(i = 0; i < S3M_NCHANNELS; i++) {
            props = d_audio_getchanprops(i);
            if(p->volume[currow][i] != 255) {
                if(p->volume[currow][i] > 0x40) p->volume[currow][i] = 0x40;
                props.volume = gvolume*p->volume[currow][i]>>6;
            }
            if(p->instrument[currow][i] > 0 && p->instrument[currow][i] != 255)
                lastinst[i] = &cursong->instruments[p->instrument[currow][i]-1];
            if(p->note[currow][i] != 255 && lastinst[i] != NULL) {
                if(p->note[currow][i] == 254)
                    d_audio_stopsample(i);
                else {
                    freq = 8363L*periods[p->note[currow][i]&0x0f];
                    freq >>= p->note[currow][i]>>4;
                    freq /= lastinst[i]->c2speed;
                    freq = 14317456L/(freq?freq:1);
                    d_audio_playsample(i, lastinst[i]->sample, freq);
                }
            }

            switch(p->command[currow][i]) {
            case 0x01:
                if(p->operand[currow][i] < 0x20) {
                    speed = p->operand[currow][i];
                }
                break;

            case 0x14:
                tempo = p->operand[currow][i];
                break;

            case 0x16:
                if(p->operand[currow][i] <= 0x40)
                    gvolume = p->operand[currow][i];
                break;
            }

            d_audio_setchanprops(i, props);
        }
        currow++;
        if(currow >= S3M_NROWS) {
            currow = 0;
            curorder++;
        }
        counter = 0;
    }
    qh = d_time_startcount(2*tempo/5, false);
}

/* EOF s3m.c */
