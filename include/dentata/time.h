/**
 * time.h
 * Created: Wed Apr  4 21:47:28 2001 by tek@wiw.org
 * Revised: Sun Jun 24 04:20:44 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: time
 *
 * Include dentata/types.h before dentata/time.h.
 */

#ifndef DENTATA_TIME_H
#define DENTATA_TIME_H

typedef void d_timehandle_t;

/**
 * d_time_wait(dword usec)
 * Sleep for a period of time.
 *
 * Takes: usec - microseconds to wait.
 */
extern void d_time_wait(dword);

/**
 * d_time_startcount(int fps, bool doprofiling)
 * Starts a new timer for frame counting. The doprofiling flag allows
 * one to instruct the timer module to return profiling information when
 * d_time_endcount() is called. See d_time_endcount().
 *
 * Takes: fps - the number of frames per second the program is attempting.
 *        doprofiling - true if extra data should be tracked to gather
 *                      profiling information.
 *
 * Returns: an opaque time handle.
 */
extern d_timehandle_t *d_time_startcount(int, bool);

/**
 * d_time_iscountfinished(d_timehandle_t *th)
 * Checks if a time handle has run over the limit or not. If it has,
 * this usually means that your program is not meeting the frame rate
 * passed to d_time_startcount().
 *
 * Takes: th - the time handle to check.
 *
 * Returns: true if the frame has run out of time, false otherwise.
 */
extern bool d_time_iscountfinished(d_timehandle_t *);

/**
 * d_time_endcount(d_timehandle_t *th)
 * Waits for a timer to finish. If doprofiling was enabled, it will
 * return the margin (in microseconds) by which your program made or
 * missed the desired time frame.
 *
 * Takes: th - the time handle to finish up. This handle is no longer
 *             valid after this function returns.
 *
 * Returns: if doprofiling was enabled when d_time_startcount() was called,
 *          this function returns an integer. If positive, it is the number
 *          of microseconds this routine had to wait. If negative, it is
 *          the number of microseconds that have gone by since the frame
 *          should have ended. If doprofiling was false, this value is
 *          undefined and may differ wildly between implementations.
 */
extern int d_time_endcount(d_timehandle_t *);

#endif

/* EOF time.h */
