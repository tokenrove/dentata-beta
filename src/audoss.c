/* 
 * audoss.c
 * Created: Sat Apr 14 23:22:27 2001 by tek@wiw.org
 * Revised: Thu Jun 28 03:45:09 2001 by tek@wiw.org
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
d_channelprops_t d_audio_getchanprops(byte);

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

#define MAXFRAGS 3
#define DSP_DEVICE "/dev/dsp"

bool d_audio_new(d_audiomode_t mode)
{
    int i;

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
    int i, j;
    long mix;
    short smix;
    d_sample_t *s;
    char *p;

    for(j = 0; j < dspbuflen;) {
        mix = 0;
        for(i = 0; i < nchannels; i++) {
            s = channels[i].sample;
            if(s == NULL || channels[i].props.mute == true)
                continue;
            switch(curmode.bitspersample) {
            case 16:
                switch(s->mode.bitspersample) {
                case 16:
                    smix = s->data[channels[i].pos]|(s->data[channels[i].pos+1]<<8);
                    smix = (smix*channels[i].props.volume)>>8;
                    mix += smix;
                    channels[i].samplenum+=channels[i].samplestep;
                    while(channels[i].samplenum >= channels[i].sampledenom) {
                        channels[i].pos+=2;
                        channels[i].samplenum -= channels[i].sampledenom;
                    }
                    break;

                case 8:
                    smix = s->data[channels[i].pos]-128;
                    smix *= channels[i].props.volume;
                    mix += smix;
                    channels[i].samplenum+=channels[i].samplestep;
                    while(channels[i].samplenum >= channels[i].sampledenom) {
                        channels[i].pos++;
                        channels[i].samplenum -= channels[i].sampledenom;
                    }
                    break;
                }
                break;

            default:
                d_error_push("d_audio_update: bad bitspersample in curmode!");
                return;
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

        switch(curmode.nchannels) {
        case 1:
            switch(curmode.bitspersample) {
            case 16:
                dspbuf[j++] = (mix)&0xFF;
                dspbuf[j++] = ((mix)>>8)&0xFF;
                break;

            default:
                d_error_fatal(__FUNCTION__": unsupported destination audio"
                              " resolution.\n");
                break;
            }
            break;

        case 2:
            switch(curmode.bitspersample) {
            case 16:
                dspbuf[j++] = (mix)&0xFF;
                dspbuf[j++] = ((mix)>>8)&0xFF;
                dspbuf[j++] = (mix)&0xFF;
                dspbuf[j++] = ((mix)>>8)&0xFF;
                break;

            default:
                d_error_fatal(__FUNCTION__": unsupported destination audio"
                              " resolution.\n");
                break;
            }
            break;
        }
    }

    p = dspbuf;
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
        d_error_push("d_audio_playsample: bad channel supplied.");
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
    channels[nchannels-1].props = props;
    channels[nchannels-1].pos = 0;
    channels[nchannels-1].sample = NULL;
    return success;
}

void d_audio_setchanprops(byte chan, d_channelprops_t props)
{
    if(chan >= nchannels) {
        d_error_push("d_audio_setchanprops: bad channel supplied.");
        return;
    }

    channels[chan].props = props;
    return;
}

d_channelprops_t d_audio_getchanprops(byte chan)
{
    if(chan >= nchannels) {
        d_error_push("d_audio_getchanprops: bad channel supplied.");
    }

    return channels[chan].props;
}

void d_audio_changesamplepos(byte chan, dword pos)
{
    channels[chan].pos = pos;
    return;
}

/* EOF audoss.c */
