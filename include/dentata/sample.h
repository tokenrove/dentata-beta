/* 
 * sample.h
 * Created: Sun Jan 28 12:40:44 2001 by tek@wiw.org
 * Revised: Sun Jan 28 14:20:57 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#ifndef DENTATA_SAMPLE_H
#define DENTATA_SAMPLE_H

#include <dentata/types.h>

typedef struct d_sample_s {
    d_audiomode_t mode;
    dword len, pos;
    byte *data;
    bool hasloop;
    dword lbegin, lend;
} d_sample_t;

extern d_sample_t *d_sample_new(d_audiomode_t, dword);
extern void        d_sample_delete(d_sample_t *);
extern d_sample_t *d_sample_dup(d_sample_t *);

#endif

/* EOF sample.h */
