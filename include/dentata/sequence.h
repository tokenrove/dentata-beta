/* 
 * sequence.h
 * Created: Mon Jul 16 07:47:14 2001 by tek@wiw.org
 * Revised: Mon Jul 16 16:14:08 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#ifndef DENTATA_SEQUENCE_H
#define DENTATA_SEQUENCE_H

typedef int (*d_comparefunc_t)(const void *, const void *);
typedef void d_sequence_t;

d_sequence_t *d_sequence_new(d_comparefunc_t cmp);
void d_sequence_delete(d_sequence_t *);


#endif

/* EOF sequence.h */
