/* 
 * tga.h
 * Created: Mon Jan 29 11:47:53 2001 by tek@wiw.org
 * Revised: Thu Apr 19 04:21:55 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Targa format support.
 *
 * Include dentata/image.h before this file.
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
