/**
 * file.h
 * Created: Mon Feb  5 15:51:30 2001 by tek@wiw.org
 * Revised: Sun Jun 24 10:15:17 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: file
 *
 * This module is not expected to change greatly, save for the
 * addition of helper functions for big endian, and some facilities
 * for saving data.
 *
 * Include dentata/types.h before dentata/file.h.
 */

#ifndef DENTATA_FILE_H
#define DENTATA_FILE_H

typedef void d_file_t;
typedef enum { current = 0, beginning = 1, end = 2 } d_fileseekrel_t;

/**
 * d_file_open(char *filename)
 * Open a file.
 *
 * Takes: filename - the name of the file to open.
 *
 * Returns: the file handle, or NULL if something went wrong.
 */
extern d_file_t *d_file_open(char *);

/**
 * d_file_close(d_file_t *fp)
 * Close a file.
 *
 * Takes: fp - the file handle.
 */
extern void      d_file_close(d_file_t *);

/**
 * d_file_getbyte(d_file_t *fp)
 * Get a byte from a file, advancing the position in the file by one byte.
 *
 * Takes: fp - the file handle.
 *
 * Returns: the byte read.
 */
extern byte      d_file_getbyte(d_file_t *);

/**
 * d_file_getword(d_file_t *fp)
 * Get a little-endian word from a file, advancing the position in the file
 * by two bytes.
 *
 * Takes: fp - the file handle.
 *
 * Returns: the word read.
 */
extern word      d_file_getword(d_file_t *);

/**
 * d_file_getdword(d_file_t *fp)
 * Get a little-endian double-word from a file, advancing the position in
 * the file by four bytes.
 *
 * Takes: fp - the file handle.
 *
 * Returns: the dword read.
 */
extern dword     d_file_getdword(d_file_t *);

/**
 * d_file_getasciizstring(d_file_t *fp)
 * Get an ASCII, NUL-terminated string from a file, advancing the position
 * in the file appropriately. Memory is allocated to hold the string, and
 * should be deallocated with d_memory_delete() after use.
 *
 * Takes: fp - the file handle.
 *
 * Returns: the string read.
 */
extern char *    d_file_getasciizstring(d_file_t *);

/**
 * d_file_seek(d_file_t *fp, int offset, d_fileseekrel_t rel)
 * Seek to a position in a file.
 *
 * Takes: fp - the file handle.
 *        offset - the number of bytes (positive or negative from rel to
 *                 seek.
 *        rel - the point of reference for seeking. Can be current, which
 *              seeks from the current point in the file, beginning, which
 *              seeks from the start of the file, and end, which seeks from
 *              the end of the file.
 */
extern void      d_file_seek(d_file_t *, int, d_fileseekrel_t);

/**
 * d_file_tell(d_file_t *fp)
 * Tells the current position in a file.
 *
 * Takes: fp - the file handle.
 *
 * Returns: the position relative to the start of the file.
 */
extern dword     d_file_tell(d_file_t *);

/**
 * d_file_read(d_file_t *fp, byte *data, dword nbytes)
 * Read bytes from a file.
 *
 * Takes: fp - the file handle.
 *        data - a pointer to the array which will hold the data read.
 *               It must have been allocated in advance, or bad things
 *               are likely to occur.
 *        nbytes - the number of bytes to read.
 *
 * Returns: the number of bytes actually read.
 */
extern dword     d_file_read(d_file_t *, byte *, dword);

#endif

/* EOF file.h */
