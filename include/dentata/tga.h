/**
 * tga.h
 * Created: Mon Jan 29 11:47:53 2001 by tek@wiw.org
 * Revised: Sat Jun 23 03:28:51 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: tga
 *
 * Include dentata/image.h before dentata/tga.h.
 */

#ifndef DENTATA_TGA_H
#define DENTATA_TGA_H

/**
 * d_tga_load(char *filename)
 * Load a targa (.tga) file.
 *
 * Takes: filename - the name of the file to load.
 *
 * Returns: a pointer to the loaded image.
 */
extern d_image_t *d_tga_load(char *);

#endif

/* EOF tga.h */
