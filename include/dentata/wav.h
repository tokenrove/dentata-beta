/**
 * wav.h
 * Created: Mon Jan 29 11:47:05 2001 by tek@wiw.org
 * Revised: Sun Jun 24 04:56:14 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: wav
 *
 * Include dentata/sample.h before this file.
 */

#ifndef DENTATA_WAV_H
#define DENTATA_WAV_H

/**
 * d_wav_load(char *filename)
 * Load an MS RIFF PCM (.wav) file.
 *
 * Takes: filename - the name of the file to load.
 *
 * Returns: a pointer to the loaded sample.
 */
extern d_sample_t *d_wav_load(char *);

#endif

/* EOF wav.h */
