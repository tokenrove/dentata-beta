/* 
 * error.h
 * Created: Mon Jan  8 04:22:57 2001 by tek@wiw.org
 * Revised: Mon Jan 29 12:10:47 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * Error handling module
 */

#include <dentata/types.h>

#ifndef DENTATA_ERROR_H
#define DENTATA_ERROR_H

extern void  d_error_push(char *);
extern char *d_error_pop(void);
extern void  d_error_dump(void);
extern void  d_error_debug(char *, ...);
extern void  d_error_fatal(char *, ...);

#endif

/* EOF error.h */
