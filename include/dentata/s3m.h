/* 
 * s3m.h
 * Created: Sun Jan 28 14:37:46 2001 by tek@wiw.org
 * Revised: Thu Apr 19 04:20:19 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta
 * S3M module
 *
 * Include dentata/types.h and dentata/sample.h before this file.
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

extern d_s3m_t *d_s3m_load(char *);
extern void     d_s3m_delete(d_s3m_t *);
extern void     d_s3m_play(d_s3m_t *);
extern void d_s3m_pause(void);
extern void d_s3m_resume(void);
extern void d_d3m_stop(void);
extern void d_s3m_update(void);

#endif

/* EOF s3m.h */
