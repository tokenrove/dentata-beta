/* 
 * filestd.c
 * Created: Sun Feb 25 03:46:39 2001 by tek@wiw.org
 * Revised: Sat Jun 23 04:09:36 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Standard C library version of dentata gen beta's file module
 */

#include <dentata/types.h>
#include <dentata/file.h>
#include <dentata/memory.h>

#include <stdio.h>

d_file_t *d_file_open(char *filename);
void d_file_close(d_file_t *fp);
byte d_file_getbyte(d_file_t *fp);
word d_file_getword(d_file_t *fp);
dword d_file_getdword(d_file_t *fp);
char *d_file_getasciizstring(d_file_t *fp);
void d_file_seek(d_file_t *fp, int offset, d_fileseekrel_t rel);
dword d_file_tell(d_file_t *fp);
dword d_file_read(d_file_t *fp, byte *data, dword len);

d_file_t *d_file_open(char *filename)
{
    FILE *fp;

    fp = fopen(filename, "rb");
    if(fp == NULL)
        return NULL;

    return (d_file_t *)fp;
}

void d_file_close(d_file_t *fp_)
{
    FILE *fp = (FILE *)fp_;

    fclose(fp);
    return;
}

byte d_file_getbyte(d_file_t *fp_)
{
    FILE *fp = (FILE *)fp_;

    return (byte)fgetc(fp);
}

word d_file_getword(d_file_t *fp)
{
    return (word)(d_file_getbyte(fp)|(d_file_getbyte(fp)<<8));
}

dword d_file_getdword(d_file_t *fp)
{
    return (dword)(d_file_getword(fp)|(d_file_getword(fp)<<16));
}

char *d_file_getasciizstring(d_file_t *fp)
{
    char *s = NULL;
    int slen = 16, i;

    s = d_memory_new(slen);
    i = 0;
    do {
        if(i >= slen) {
            slen *= 2;
            s = d_memory_resize(s, slen);
        }
        s[i] = d_file_getbyte(fp);
    } while(s[i++]);

    return s;
}

void d_file_seek(d_file_t *fp_, int offset, d_fileseekrel_t rel)
{
    FILE *fp = (FILE *)fp_;

    fseek(fp, offset, (rel == current)?SEEK_CUR:
                      (rel == beginning)?SEEK_SET:SEEK_END);
    return;
}

dword d_file_tell(d_file_t *fp)
{
    return ftell(fp);
}

dword d_file_read(d_file_t *fp_, byte *data, dword len)
{
    FILE *fp = (FILE *)fp_;

    return fread(data, len, 1, fp);
}

/* EOF filestd.c */
