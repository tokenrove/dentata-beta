/* 
 * audnul.c
 * Created: Thu Jul 12 15:44:43 2001 by tek@wiw.org
 * Revised: Thu Jul 12 15:44:43 2001 (pending)
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/audio.h>

bool d_audio_new(d_audiomode_t);
void d_audio_delete(void);
void d_audio_update(void);
void d_audio_playsample(byte, d_sample_t *, dword);
void d_audio_stopsample(byte);
int  d_audio_nchannels(void);
bool d_audio_addchannel(d_channelprops_t);
void d_audio_setchanprops(byte, d_channelprops_t);
d_channelprops_t d_audio_getchanprops(byte);
void d_audio_changesamplepos(byte, dword);

bool d_audio_new(d_audiomode_t)
{
    return failure;
}

void d_audio_delete(void) { }
void d_audio_update(void) { }
void d_audio_playsample(byte, d_sample_t *, dword) { }
void d_audio_stopsample(byte) { }
int  d_audio_nchannels(void) { return 0; } 
bool d_audio_addchannel(d_channelprops_t) { return failure; }
void d_audio_setchanprops(byte, d_channelprops_t) { }

d_channelprops_t d_audio_getchanprops(byte)
{
    d_channelprops_t foo;
    return foo;
}

void d_audio_changesamplepos(byte, dword) { }

/* EOF audnul.c */
