/**
 * random.h
 * Created: Wed Jan 24 13:30:19 2001 by tek@wiw.org
 * Revised: Sat Jun 23 03:26:43 2001 by tek@wiw.org
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

extern d_random_t *d_random_new(void);
extern void        d_random_delete(d_random_t *);
extern dword       d_random_gen(d_random_t *, dword);

#endif

/* EOF random.h */
