/* 
 * time.h
 * Created: Wed Apr  4 21:47:28 2001 by tek@wiw.org
 * Revised: Sun Apr 15 18:34:15 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#ifndef DENTATA_TIME_H
#define DENTATA_TIME_H

#include <dentata/types.h>

extern void d_time_wait(dword);
extern void *d_time_startcount(int);
extern bool d_time_iscountfinished(void *);
extern void d_time_endcount(void *);

#endif

/* EOF time.h */
