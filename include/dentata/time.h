/* 
 * time.h
 * Created: Wed Apr  4 21:47:28 2001 by tek@wiw.org
 * Revised: Thu Apr 19 04:22:12 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Timer support
 *
 * Include dentata/types.h before this file.
 */

#ifndef DENTATA_TIME_H
#define DENTATA_TIME_H

extern void d_time_wait(dword);
extern void *d_time_startcount(int);
extern bool d_time_iscountfinished(void *);
extern void d_time_endcount(void *);

#endif

/* EOF time.h */
