/* 
 * audoss.c
 * Created: Sat Apr 14 23:22:27 2001 by tek@wiw.org
 * Revised: Fri Jul 13 03:37:48 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/sample.h>
#include <dentata/audio.h>
#include <dentata/error.h>
#include <dentata/memory.h>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

bool d_audio_new(d_audiomode_t);
void d_audio_delete(void);
void d_audio_update(void);
void d_audio_playsample(byte, d_sample_t *, dword);
void d_audio_stopsample(byte);
int d_audio_nchannels(void);
bool d_audio_addchannel(d_channelprops_t);
void d_audio_setchanprops(byte, d_channelprops_t);
void d_audio_getchanprops(byte, d_channelprops_t *);

typedef struct dspchannel_s {
    dword pos;
    dword samplestep, samplenum, sampledenom;
    d_sample_t *sample;
    d_channelprops_t props;
} dspchannel_t;

static int dspfd;
static int nchannels;
static dspchannel_t *channels;
static word dspbuflen;
static byte *dspbuf;
static d_audiomode_t curmode;

static short volumetable[D_MAXVOLUME+1][256];

#define MAXFRAGS 3
#define DSP_DEVICE "/dev/dsp"

bool d_audio_new(d_audiomode_t mode)
{
    int i, j;

    for(i = 0; i <= D_MAXVOLUME; i++)
        for(j = 0; j < 256; j++)
            volumetable[i][j] = i*(j-128)/64;

    /* FIXME: This might need some tweaking */
    dspbuflen = (mode.frequency < 22050)?4:(mode.frequency > 32000)?6:5;
    if(mode.nchannels == 2) dspbuflen++;
    if(mode.bitspersample == 16) dspbuflen++;

    dspfd = open(DSP_DEVICE, O_WRONLY, 0);
    if(dspfd == -1) {
        d_error_push("d_audio_new: Unable to open DSP device ("
                     DSP_DEVICE ").");
        return failure;
    }

    i = (MAXFRAGS<<16)|dspbuflen;
    if(ioctl(dspfd, SNDCTL_DSP_SETFRAGMENT, &i) == -1) {
        d_error_push("d_audio_new: Unable to set fragment size.");        
        goto error;
    }
    ioctl(dspfd, SNDCTL_DSP_GETBLKSIZE, &dspbuflen);

    switch(mode.bitspersample) {
    case 8:
        i = AFMT_S8;
        break;
    case 16:
        i = AFMT_S16_LE;
        break;
    default:
        d_error_push("d_audio_new: Unsupported bits-per-sample.");
        goto error;
    }

    if(ioctl(dspfd, SNDCTL_DSP_SETFMT, &i) == -1) {
        d_error_push("d_audio_new: Unable to set format.");
        goto error;
    }

    switch(mode.nchannels) {
    case 1:
    case 2:
        i = mode.nchannels;
        break;
    default:
        d_error_push("d_audio_new: Unsupported number of channels.");
        goto error;
    }

    if(ioctl(dspfd, SNDCTL_DSP_CHANNELS, &i) == -1) {
        d_error_push("d_audio_new: Unable to set number of channels.");
        goto error;
    }

    i = mode.frequency;
    if(ioctl(dspfd, SNDCTL_DSP_SPEED, &i) == -1) {
        d_error_push("d_audio_new: Unable to set rate.");
        goto error;
    }

    nchannels = 0;
    channels = NULL;
    dspbuf = d_memory_new(dspbuflen);
    if(dspbuf == NULL) {
        d_error_push("d_audio_new: Failed to allocate DSP buffer.");
        goto error;
    }

    curmode = mode;

    return success;

error:
    close(dspfd);
    return failure;
}

void d_audio_delete(void)
{
    if(nchannels > 0)
        d_memory_delete(channels);
    close(dspfd);
    return;
}

void d_audio_update(void)
{
    int i, j, a, b, c;
    short mix;
    d_sample_t *s;
    char *p;

    c = 2048*16/48;
    a = (2048-c)/2;
    b = a+c;

    for(j = 0; j < dspbuflen;) {
        mix = 1024;
        for(i = 0; i < nchannels; i++) {
            s = channels[i].sample;
            if(s == NULL || channels[i].props.mute == true)
                continue;
            mix += volumetable[channels[i].props.volume][s->data[channels[i].pos]];
            channels[i].samplenum+=channels[i].samplestep;
            while(channels[i].samplenum >= channels[i].sampledenom) {
                channels[i].pos++;
                channels[i].samplenum -= channels[i].sampledenom;
            }

            if(channels[i].pos >= s->len ||
               (s->hasloop && channels[i].pos >= s->lend)) {
                if(s->hasloop) {
                    channels[i].pos = s->lbegin;
                } else {
                    channels[i].pos = 0;
                    channels[i].sample = NULL;
                }
            }
        }

        if(mix < a) mix = 0;
        else if(mix > b) mix = 255;
        else {
            mix -= 1024;
            mix = (mix-a)*256/(b-a);
        }

        dspbuf[j++] = mix+128;
    }

    p = (char *)dspbuf;
    for(i = dspbuflen; i > 0;) {
        j = write(dspfd, p, i);
        if(j > 0) {
            i -= j;
            p += j;
        } else
            break;
    }
    d_memory_set(dspbuf, 0, dspbuflen);
    return;
}

void d_audio_playsample(byte chan, d_sample_t *p, dword frequency)
{
    if(chan >= nchannels) {
        d_error_push("bad channel supplied.");
        return;
    }

    if(p->mode.bitspersample != 8 || p->mode.nchannels != 1 ||
       p->mode.encoding != PCM) {
        d_error_push("unsupported sample parameters.");
        return;
    }

    channels[chan].pos = 0;
    channels[chan].sample = p;
    channels[chan].samplenum = 0;
    channels[chan].sampledenom = curmode.frequency;
    channels[chan].samplestep = frequency;
    return;
}

void d_audio_stopsample(byte chan)
{
    if(chan >= nchannels) {
        d_error_push("d_audio_stopsample: bad channel supplied.");
        return;
    }

    channels[chan].pos = 0;
    channels[chan].sample = NULL;
    return;
}

int d_audio_nchannels(void)
{
    return nchannels;
}

bool d_audio_addchannel(d_channelprops_t props)
{
    dspchannel_t *tmp;

    tmp = d_memory_resize(channels, (nchannels+1)*sizeof(dspchannel_t));
    if(tmp == NULL)
        return failure;
    channels = tmp;
    nchannels++;
    if(props.volume > D_MAXVOLUME) props.volume = D_MAXVOLUME;
    channels[nchannels-1].props = props;
    channels[nchannels-1].pos = 0;
    channels[nchannels-1].sample = NULL;
    return success;
}

void d_audio_setchanprops(byte chan, d_channelprops_t props)
{
    if(chan >= nchannels) {
        d_error_push("bad channel supplied.");
        return;
    }

    if(props.volume > D_MAXVOLUME) props.volume = D_MAXVOLUME;
    channels[chan].props = props;
    return;
}

void d_audio_getchanprops(byte chan, d_channelprops_t *props)
{
    if(props == NULL) {
        d_error_push("slipped a NULL pointer.");
        return;
    }

    if(chan >= nchannels) {
        d_error_push("bad channel supplied.");
        return;
    }

    *props = channels[chan].props;
}

void d_audio_setsamplepos(byte chan, dword pos)
{
    if(chan >= nchannels) {
        d_error_push("bad channel supplied.");
        return;
    }

    channels[chan].pos = pos;
    return;
}

void d_audio_getsamplepos(byte chan, dword *pos)
{
    if(chan >= nchannels) {
        d_error_push("bad channel supplied.");
        return;
    }

    *pos = channels[chan].pos;
    return;
}

/* EOF audoss.c */
