/* 
 * raster.h
 * Created: Sat Jan  8 23:40:41 2000 by tek@wiw.org
 * Revised: Fri Jun 22 19:42:38 2001 by tek@wiw.org
 * Copyright 2000 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * Raster module
 *
 * Include dentata/types.h before this file.
 */

#include <dentata/types.h>

#ifndef DENTATA_RASTER_H
#define DENTATA_RASTER_H

/**
 * d_raster_new()
 * Initializes the raster module. Note that it is not valid to assume
 * that the hardware will be initialized to a valid mode.
 *
 * Returns: failure if something went wrong, success otherwise.
 */
extern bool                   d_raster_new(void);

/**
 * d_raster_delete()
 * Shut down the raster module. Returns the system to its original
 * graphics mode if appropriate.
 */
extern void                   d_raster_delete(void);

/**
 * d_raster_setmode(d_rasterdescription_t mode)
 * Brings the raster hardware into the specified mode, sets up the internal
 * double buffer for this mode.
 *
 * Takes: mode - a description of the mode to attempt.
 *
 * Returns: success if the mode change succeeded, failure otherwise.
 */
extern bool                   d_raster_setmode(d_rasterdescription_t);

/**
 * d_raster_getmodes(int *nmodes)
 * Gets a list of modes accessable on this raster device.
 *
 * Takes: nmodes - a pointer to an integer in which the number of modes
 *                 available will be stored.
 *
 * Returns: An array of nmodes raster descriptions describing each
 *          available mode.
 */
extern d_rasterdescription_t *d_raster_getmodes(int *);

/**
 * d_raster_getcurrentmode()
 * Gets information about the current mode.
 *
 * Returns: a description of the currently active mode.
 */
extern d_rasterdescription_t  d_raster_getcurrentmode(void);

/**
 * d_raster_getgfxptr()
 * Gets a pointer to the internal double buffer.
 *
 * Returns: a pointer to the internal double buffer.
 */
extern byte *                 d_raster_getgfxptr(void);

/**
 * d_raster_update()
 * Synchronizes the raster display with the internal double buffer.
 * This must be called once per frame in order to get any form of
 * display.
 */
extern void                   d_raster_update(void);

/**
 * d_raster_setpalette(d_palette_t *palette)
 * Sets the hardware palette. This call is only valid if the current
 * mode is paletted.
 *
 * Takes: palette - the palette to use. This might lose precision when
 *                  being converted to a format more amenable to the
 *                  hardware.
 *
 * Pushes an error if this is called for a non-paletted mode.
 */
extern void                   d_raster_setpalette(d_palette_t *);

/**
 * d_raster_getpalette()
 * Gets the hardware palette. Note that it is not safe to assume this
 * call is valid before d_raster_setpalette() has been called, nor
 * is it valid to call this when a non-paletted mode is set.
 *
 * Returns: the current palette in use.
 *
 * Pushes an error if this is called for a non-paletted mode or if the
 * palette has not previously been initialized.
 */
extern void                   d_raster_getpalette(d_palette_t *);

#endif

/* EOF raster.h */
