/* 
 * file.h
 * Created: Mon Feb  5 15:51:30 2001 by tek@wiw.org
 * Revised: Mon Feb  5 15:51:30 2001 (pending)
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#ifndef DENTATA_FILE_H
#define DENTATA_FILE_H

#include <dentata/types.h>

typedef void d_file_t;

d_file_t *d_file_open(char *);
void      d_file_close(d_file_t *);

byte      d_file_getbyte(d_file_t *);
word      d_file_getword(d_file_t *);
dword     d_file_getdword(d_file_t *);
char *    d_file_getasciizstring(d_file_t *);

#endif

/* EOF file.h */
