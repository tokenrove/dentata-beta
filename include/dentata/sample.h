/**
 * sample.h
 * Created: Sun Jan 28 12:40:44 2001 by tek@wiw.org
 * Revised: Sun Jun 24 03:02:50 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: sample
 *
 * Notable future additions to this module will include sample
 * processing features such as format conversion, effects, and
 * filters.
 *
 * Include dentata/types.h before dentata/sample.h.
 */

#ifndef DENTATA_SAMPLE_H
#define DENTATA_SAMPLE_H

typedef struct d_sample_s {
    d_audiomode_t mode;
    dword len;
    byte *data;
    bool hasloop;
    dword lbegin, lend;
} d_sample_t;

/**
 * d_sample_new(d_audiomode_t mode)
 * Creates a new audio sample.
 *
 * Takes: mode - the parameters by which to create this sample.
 *
 * Returns: the newly allocated sample.
 */
extern d_sample_t *d_sample_new(d_audiomode_t);

/**
 * d_sample_delete(d_sample_t *p)
 * Deallocates an audio sample.
 *
 * Takes: p - the sample to delete.
 */
extern void        d_sample_delete(d_sample_t *);

/**
 * d_sample_dup(d_sample_t *p)
 * Makes a deep copy of an audio sample.
 *
 * Takes: p - the sample to duplicate.
 *
 * Returns: the new copy of p.
 */
extern d_sample_t *d_sample_dup(d_sample_t *);

#endif

/* EOF sample.h */
