/* 
 * sequence.h
 * Created: Mon Jul 16 07:47:14 2001 by tek@wiw.org
 * Revised: Mon Jul 16 22:13:15 2001 by tek@wiw.org
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
bool d_sequence_add(d_sequence_t *, dword, void *);
bool d_sequence_remove(d_sequence_t *, dword);
bool d_sequence_fetch(d_sequence_t *, dword, void **);
dword d_sequence_nelements(d_sequence_t *);
void d_sequence_resetiteration(d_sequence_t *);
dword d_sequence_nextkey(d_sequence_t *);

#endif

/* EOF sequence.h */
