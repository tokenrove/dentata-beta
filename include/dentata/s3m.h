/**
 * s3m.h
 * Created: Sun Jan 28 14:37:46 2001 by tek@wiw.org
 * Revised: Mon Jul 16 02:54:39 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: s3m
 *
 * Directly calling this module will become deprecated in the
 * near future upon completion of the music module, which will
 * provide more platform and format independant music playback.
 * Another change will be the addition of triggers, so that
 * functions can be called every time a specific musical event
 * occurs.
 *
 * Include dentata/types.h and dentata/sample.h before dentata/s3m.h.
 */

#ifndef DENTATA_S3M_H
#define DENTATA_S3M_H

#define S3M_NCHANNELS     32
#define S3M_NROWS         64
#define S3M_TITLELEN      28
#define S3M_FILENAMELEN   12
#define S3M_INSTNAMELEN   28
#define S3M_ADLIBSPECLEN  12
#define S3M_INSTRUMENTLEN 80

typedef struct d_s3m_instrument_s {
    enum { sample = 1, amel = 2, abassdrum = 3, asnare = 4, atom = 5,
           acym = 6, ahihat = 7 } type;
    byte filename[S3M_FILENAMELEN], name[S3M_INSTNAMELEN];
    dword c2speed;
    byte volume, pack, flags;
    byte adlibspecs[S3M_ADLIBSPECLEN];
    d_sample_t *sample;
} d_s3m_instrument_t;

typedef struct d_s3m_pattern_s {
    byte note[S3M_NROWS][S3M_NCHANNELS];
    byte instrument[S3M_NROWS][S3M_NCHANNELS];
    byte volume[S3M_NROWS][S3M_NCHANNELS];
    byte command[S3M_NROWS][S3M_NCHANNELS];
    byte operand[S3M_NROWS][S3M_NCHANNELS];
} d_s3m_pattern_t;

typedef struct d_s3m_s {
    byte name[S3M_TITLELEN];
    word norders, npatterns, ninstruments;
    word flags, ffi;
    byte gvolume, mvolume, speed, tempo;
    byte ultraclick, usedefaultpans;
    byte channelset[S3M_NCHANNELS];
    byte channelpan[S3M_NCHANNELS];

    byte *orders;
    d_s3m_pattern_t *patterns;
    d_s3m_instrument_t *instruments;
} d_s3m_t;

typedef void d_s3mhandle_t;

/**
 * d_s3m_load(char *filename)
 * Loads a screamtracker 3 module.
 *
 * Takes: filename - the name of the module to load.
 *
 * Returns: the s3m structure of the loaded module, or NULL if something
 *          went wrong.
 */
extern d_s3m_t *d_s3m_load(char *);

/**
 * d_s3m_delete(d_s3m_t *p)
 * Frees the memory taken up by an S3M.
 *
 * Takes: p - the module to free.
 */
extern void d_s3m_delete(d_s3m_t *);

/**
 * d_s3m_play(d_s3m_t *p)
 * Begins playing an S3M. This will obviously cause quite a few errors
 * if it is attempted without initializing the audio device. See also
 * d_s3m_update(). Note that currently, playing multiple s3ms simultaneously
 * is impossible.
 *
 * Takes: p - the module to play.
 *
 * Returns: a handle by which to refer to this s3m.
 */
extern d_s3mhandle_t *d_s3m_play(d_s3m_t *);

/**
 * d_s3m_stop(d_s3mhandle_t *sh)
 * Stops a playing or paused s3m and frees the handle.
 *
 * Takes: sh - the handle which refers to this s3m. No longer valid
 *             after this function has been called.
 */
extern void d_s3m_stop(d_s3mhandle_t *);

/**
 * d_s3m_update(d_s3mhandle_t *sh)
 * Updates an s3m. This should be called once per frame, before
 * d_audio_update().
 *
 * Takes: sh - the handle which refers to this s3m.
 *
 * Returns: true if the song has completed playback, false otherwise.
 */
extern bool d_s3m_update(d_s3mhandle_t *);

/**
 * d_s3m_pause(d_s3mhandle_t *sh)
 * Pauses a playing s3m. No effect if the s3m is already paused.
 *
 * Takes: sh - the handle which refers to this s3m.
 */
extern void d_s3m_pause(d_s3mhandle_t *);

/**
 * d_s3m_resume(d_s3mhandle_t *sh)
 * Resumes a paused s3m. No effect if the s3m is already playing.
 *
 * Takes: sh - the handle which refers to this s3m.
 */
extern void d_s3m_resume(d_s3mhandle_t *);

#endif

/* EOF s3m.h */
