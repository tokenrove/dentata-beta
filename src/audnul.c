/* 
 * audnul.c
 * Created: Thu Jul 12 15:44:43 2001 by tek@wiw.org
 * Revised: Fri Jul 13 03:05:23 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/sample.h>
#include <dentata/audio.h>

bool d_audio_new(d_audiomode_t);
void d_audio_delete(void);
void d_audio_update(void);
void d_audio_playsample(byte, d_sample_t *, dword);
void d_audio_stopsample(byte);
int  d_audio_nchannels(void);
bool d_audio_addchannel(d_channelprops_t);
void d_audio_setchanprops(byte, d_channelprops_t);
void d_audio_getchanprops(byte, d_channelprops_t *);
void d_audio_setsamplepos(byte, dword);
void d_audio_getsamplepos(byte, dword *);

bool d_audio_new(d_audiomode_t mode)
{
    return failure;
}

void d_audio_delete(void) { }
void d_audio_update(void) { }
void d_audio_playsample(byte chan, d_sample_t *s, dword freq) { }
void d_audio_stopsample(byte chan) { }
int  d_audio_nchannels(void) { return 0; } 
bool d_audio_addchannel(d_channelprops_t props) { return failure; }
void d_audio_setchanprops(byte chan, d_channelprops_t props) { }
void d_audio_getchanprops(byte chan, d_channelprops_t *props) { }
void d_audio_setsamplepos(byte chan, dword pos) { }
void d_audio_getsamplepos(byte chan, dword *pos) { }

/* EOF audnul.c */
