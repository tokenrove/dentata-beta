/* 
 * plays3m.c
 * Created: Wed Jun 27 08:55:21 2001 by tek@wiw.org
 * Revised: Wed Jun 27 09:05:39 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/sample.h>
#include <dentata/audio.h>
#include <dentata/s3m.h>
#include <dentata/error.h>

int main(int argc, char **argv)
{
    d_audiomode_t mode;
    bool status;
    int i;
    char *filename = NULL;
    d_s3m_t *s3m;
    d_s3mhandle_t *sh;

    mode.frequency = 22050;
    mode.bitspersample = 16;
    mode.nchannels = 1;
    mode.encoding = PCM;

    for(i = 1; i < argc; i++) {
        filename = argv[i];
    }

    if(filename == NULL)
        d_error_fatal("No s3m file specified.\n");

    status = d_audio_new(mode);
    if(status == failure)
        d_error_fatal("Failed to initialize audio driver.\n");

    s3m = d_s3m_load(filename);
    if(s3m == NULL)
        d_error_fatal("Failed to load ``%s''.\n", filename);

    sh = d_s3m_play(s3m);
    for(i = 0; ; i++) {
        d_s3m_update(sh);
        d_audio_update();
    }
    d_s3m_stop(sh);
    
    d_s3m_delete(s3m);
    d_audio_delete();
    return 0;
}

/* EOF plays3m.c */
