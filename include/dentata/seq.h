/* 
 * seq.h ($Id$)
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * 
 */

#ifndef DENTATA_SEQ_H
#define DENTATA_SEQ_H

typedef int (*d_comparefunc_t)(const void *, const void *);
typedef void d_seq_t;

extern d_seq_t *d_seq_new(d_comparefunc_t);
extern void d_seq_delete(d_seq_t *);
extern bool d_seq_add(d_seq_t *, void *, void *);
extern bool d_seq_remove(d_seq_t *, void *);
extern bool d_seq_fetch(d_seq_t *, void *, void **);
extern dword d_seq_nelements(d_seq_t *);
extern void *d_seq_nextkey(d_iterator_t *, d_seq_t *);

#endif

/* EOF seq.h */
