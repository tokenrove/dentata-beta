/**
 * random.h
 * Created: Wed Jan 24 13:30:19 2001 by tek@wiw.org
 * Revised: Sun Jun 24 02:49:04 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: random
 *
 * Include dentata/types.h before this file.
 */

#ifndef DENTATA_RANDOM_H
#define DENTATA_RANDOM_H

typedef void d_random_t;

/**
 * d_random_new(void)
 * Creates a new random handle.
 *
 * Returns: the new random handle.
 */
extern d_random_t *d_random_new(void);

/**
 * d_random_delete(d_random_t *rh)
 * Deletes a random handle.
 *
 * Takes: rh - the random handle to delete.
 */
extern void d_random_delete(d_random_t *);

/**
 * d_random_gen(d_random_t *rh, dword max)
 * Generates a random integer between 0 and max-1.
 *
 * Takes: rh - the random handle to use.
 *        max - specifies the limit of the number returned.
 *
 * Returns: a random integer between 0 and max-1.
 */
extern dword d_random_gen(d_random_t *, dword);

#endif

/* EOF random.h */
