/* 
 * errorstd.c
 * Created: Mon Jan  8 05:02:54 2001 by tek@wiw.org
 * Revised: Sat Jun 23 04:07:10 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * Error handling module system specific bits, stdlib wrapper version
 */

#include <dentata/error.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void d_error_dump(void);
void d_error_debug(char *, ...);
void d_error_fatal(char *, ...);

void d_error_dump(void)
{
    char *s;

    while(s = d_error_pop(), s != NULL) {
        fprintf(stderr, "Error: %s\n", s);
    }
    return;
}

void d_error_debug(char *fmt, ...)
{
    va_list ap;

#ifdef DEBUG
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
#endif
}

void d_error_fatal(char *fmt, ...)
{
    va_list ap;

    d_error_dump();
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}

/* EOF errorstd.c */
