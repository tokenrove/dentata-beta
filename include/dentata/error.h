/**
 * error.h
 * Created: Mon Jan  8 04:22:57 2001 by tek@wiw.org
 * Revised: Sun Jun 24 10:14:50 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: error
 *
 * Note: This module is expected to increase in complexity as error
 * values and callbacks are integrated.
 *
 * Include dentata/types.h before dentata/error.h. 
 */

#ifndef DENTATA_ERROR_H
#define DENTATA_ERROR_H

/**
 * d_error_push(char *msg)
 * Push an error onto the error stack. Usually the error stack is reserved
 * for errors that are exceptional cases - routine information should be
 * returned via return values and parameters. The error stack is limited
 * to avoid having too many errors pile up unnoticed, and to avoid dependancy
 * on the memory module, which is a potential source of error messages.
 * If the error stack overflows, a fatal error occurs (see d_event_fatal()).
 *
 * Takes: msg - the error message to push. The accepted format is
 *              "routine: message.". Note there is a full stop, but no
 *              newline.
 */
extern void  d_error_push(char *);

/**
 * d_error_pop()
 * Pops the top (most recent) error off the error stack.
 *
 * Returns: a character pointer to the message popped, or NULL if the stack
 *          is empty.
 */
extern char *d_error_pop(void);

/**
 * d_error_dump()
 * Dumps the error stack in whatever way is most appropriate for the
 * current system. The error stack is wiped by this operation.
 */
extern void  d_error_dump(void);

/**
 * d_error_debug(char *msg, ...)
 * Displays a debugging error message. These messages only appear if
 * the library was compiled with debugging turned on.
 *
 * Takes: the same options as printf(3).
 */
extern void  d_error_debug(char *, ...);

/**
 * d_error_fatal(char *msg, ...)
 * Dumps the error stack, displays a fatal error message, and prematurely
 * terminates the program.
 *
 * Takes: the same options as printf(3).
 */
extern void  d_error_fatal(char *, ...);

#endif

/* EOF error.h */
