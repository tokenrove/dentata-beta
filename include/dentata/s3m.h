/* 
 * s3m.h
 * Created: Sun Jan 28 14:37:46 2001 by tek@wiw.org
 * Revised: Mon Jan 29 13:38:42 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata gen Beta
 * S3M module
 */

#ifndef DENTATA_S3M_H
#define DENTATA_S3M_H

#include <dentata/types.h>
#include <dentata/sample.h>

#define D_S3M_PATLEN 64

typedef struct d_s3m_pattern_s {
    /* some stuff[D_S3M_PATLEN] */
} d_s3m_pattern_t;

typedef struct d_s3m_s {
    byte nchannels;

    word npatterns;
    d_s3m_pattern_t *patterns;

    word norders, curorder, curpos;
    word *orders;

    word nsamples;
    d_sample_t **samples;
} d_s3m_t;

extern d_s3m_t *d_s3m_load(char *);
extern void     d_s3m_delete(d_s3m_t *);
extern void     d_s3m_update(d_s3m_t *);

#endif

/* EOF s3m.h */
