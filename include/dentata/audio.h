/**
 * audio.h
 * Created: Thu Jan 25 12:09:37 2001 by tek@wiw.org
 * Revised: Sun Jun 24 10:13:30 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: audio
 *
 * Note: Querying the number of real channels is a feature which
 * will be added in the near future. Currently there is no distinction
 * between real and virtual channels, for a user, other than their
 * resource usage.
 *
 * Include dentata/types.h and dentata/sample.h before dentata/audio.h.
 */

#ifndef DENTATA_AUDIO_H
#define DENTATA_AUDIO_H

/**
 * d_channelprops_t
 * Audio channel property structure.
 *
 * Members: volume - Loudness of this channel in the mix.
 *          panning - In a stereo recording, determines how much of
 *                    the channel is mixed into the right or left
 *                    speaker. Ranges from -127 (far left) to 127 (far right),
 *                    with 0 being perfectly balanced (default).
 *          mute - True if this channel is being ignored in the mix, false
 *                 if it is included.
 */
typedef struct d_channelprops_s {
    byte volume;
    signed char panning;
    bool mute;
} d_channelprops_t;

/**
 * d_audio_new(d_audiomode_t mode)
 * Setup the audio device in the specified mode.
 *
 * Takes: mode - the audio mode.
 *
 * Returns: success if the audio device was successfully initialized,
 *          failure otherwise.
 */
extern bool d_audio_new(d_audiomode_t);

/**
 * d_audio_delete(void)
 * Shut down the audio device.
 */
extern void d_audio_delete(void);

/**
 * d_audio_update(void)
 * Update the audio device. This must be called at least once per
 * frame on some systems (and the frame rate must be high enough for
 * the current audio mode -- see the platform details for your system),
 * while on others it performs no operation. Portable code must call
 * it once per frame at a minimum.
 */
extern void d_audio_update(void);

/**
 * d_audio_playsample(byte channel, d_sample_t *sample, dword frequency)
 * Starts playing a sample on the specified channel, mixed at the specified
 * frequency. This will interrupt any currently playing sample on this
 * channel.
 *
 * If you aren't sure what frequency to supply, use sample->mode.frequency.
 *
 * Takes: channel - the channel on which to play the sample.
 *        sample - the sample to play.
 *        frequency - the frequency at which to mix the sample.
 */
extern void d_audio_playsample(byte, d_sample_t *, dword);

/**
 * d_audio_stopsample(byte channel)
 * Stops playing a sample on a channel, if a sample is playing. Otherwise
 * it has no effect.
 *
 * Takes: channel - the channel to affect.
 */
extern void d_audio_stopsample(byte);

/**
 * d_audio_nchannels(void)
 * Get the current number of available audio channels. Both real
 * and virtual channels are included.
 *
 * Returns: the number of available audio channels.
 */
extern int  d_audio_nchannels(void);

/**
 * d_audio_addchannel(d_channelprops_t props)
 * Adds a new audio channel. This may be a real or a virtual channel.
 *
 * Takes: props - the initial properties of this channel.
 *
 * Returns: success if the channel was added, failure otherwise.
 */
extern bool d_audio_addchannel(d_channelprops_t);

/**
 * d_audio_setchanprops(byte channel, d_channelprops_t props)
 * Sets the properties for an audio channel.
 *
 * Takes: channel - the channel to affect.
 *        props - the new properties to use.
 */
extern void d_audio_setchanprops(byte, d_channelprops_t);

/**
 * d_audio_getchanprops(byte channel)
 * Gets the properties of an audio channel.
 *
 * Takes: channel - the channel from which to read.
 *
 * Returns: the current channel properties.
 */
extern d_channelprops_t d_audio_getchanprops(byte);

#endif

/* EOF audio.h */
