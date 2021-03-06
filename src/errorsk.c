/* 
 * errorsk.c
 * Created: Mon Jan  8 04:22:24 2001 by tek@wiw.org
 * Revised: Sat Jun 23 04:13:22 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * Error module - system independant error stack
 */

#include <dentata/types.h>
#include <dentata/error.h>

/* This is intentionally limited so that after enough ignored warnings,
 * the program dies. */
#define ERRSTACK_DEPTH 25

void d_error_push(char *);
char *d_error_pop(void);

/* static variables */
static char *error_stack[ERRSTACK_DEPTH];
static int error_sp = 0;

void d_error_push(char *msg)
{
    if(error_sp >= ERRSTACK_DEPTH) {
        d_error_fatal("Error stack out of space!");
    }

    error_stack[error_sp++] = msg;
    return;
}

char *d_error_pop(void)
{
    if(error_sp == 0) return NULL;
    return error_stack[--error_sp];
}

/* EOF errorsk.c */
