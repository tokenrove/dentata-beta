/**
 * pcx.h
 * Created: Mon Feb  5 16:19:15 2001 by tek@wiw.org
 * Revised: Sat Jun 23 03:27:44 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: pcx
 *
 * Include dentata/image.h before dentata/pcx.h.
 */

#ifndef DENTATA_PCX_H
#define DENTATA_PCX_H

/**
 * d_pcx_load(char *filename)
 * Load a PC Paintbrush (.pcx) file.
 *
 * Takes: filename - the name of the file to load.
 *
 * Returns: a pointer to the loaded image.
 */
extern d_image_t *d_pcx_load(char *);

#endif

/* EOF pcx.h */
