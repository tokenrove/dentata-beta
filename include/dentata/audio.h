/* 
 * audio.h
 * Created: Thu Jan 25 12:09:37 2001 by tek@wiw.org
 * Revised: Thu Apr 19 04:13:14 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta
 * Base audio module
 *
 * Include dentata/types.h and dentata/sample.h before this file.
 */

#ifndef DENTATA_AUDIO_H
#define DENTATA_AUDIO_H

typedef struct d_channelprops_s {
    byte volume;
    signed char panning;
    bool mute;
} d_channelprops_t;

extern bool d_audio_new(d_audiomode_t);
extern void d_audio_delete(void);
extern void d_audio_update(void);
extern void d_audio_playsample(byte, d_sample_t *, dword);
extern void d_audio_stopsample(byte);
extern int  d_audio_nchannels(void);
extern bool d_audio_addchannel(d_channelprops_t);
extern void d_audio_setchanprops(byte, d_channelprops_t);
extern d_channelprops_t d_audio_getchanprops(byte);

#endif

/* EOF audio.h */
