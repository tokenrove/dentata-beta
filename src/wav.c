/* 
 * wav.c
 * Created: Sun Apr 15 04:02:28 2001 by tek@wiw.org
 * Revised: Sun Apr 15 04:45:06 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/sample.h>
#include <dentata/wav.h>
#include <dentata/file.h>
#include <dentata/memory.h>

d_sample_t *d_wav_load(char *filename)
{
    d_sample_t *p;
    d_file_t *fp;
    dword x;
    int len;
    d_audiomode_t mode;

    p = NULL;

    fp = d_file_open(filename);
    if(fp == NULL)
        return NULL;

    x = d_file_getdword(fp);
    if(x != 0x46464952) return NULL; /* RIFF */
    len = d_file_getdword(fp);

    x = d_file_getdword(fp); len -= 4;
    /* FIXME should support non-canonical RIFFs */
    if(x != 0x45564157) return NULL; /* WAVE */

    while(len > 0) {
        x = d_file_getdword(fp); len-=4;
        if(x == 0x20746D66) { /* fmt  */
            x = d_file_getdword(fp); /* size */
            x = d_file_getword(fp);
            /* FIXME: no support for non-PCM */
            if(x != 0x0001) return NULL;
            mode.encoding = PCM;
            mode.nchannels = d_file_getword(fp);
            mode.frequency = d_file_getdword(fp);
            d_file_seek(fp, 4, current); /* avg bytes per second */
            d_file_seek(fp, 2, current); /* block alignment */
            mode.bitspersample = d_file_getword(fp);

            len -= 18;

            p = d_sample_new(mode);
            if(p == NULL) return NULL;
            
        } else if(x == 0x61746164) { /* data */
            if(p == NULL) return NULL;
            x = d_file_getdword(fp); len -= 4;
            p->data = d_memory_new(x);
            d_file_read(fp, p->data, x);
            p->len = x; len -= x;
            return p;

        } else if(x == 0xffffffff) { /* probably eof */
            return p;

        } else { /* unsupported chunk (skip) */
            x = d_file_getdword(fp); len -= 4;
            d_file_seek(fp, x, current); len -= x;
        }
    }

    d_file_close(fp);
    return p;
}

/* EOF wav.c */
