/* 
 * listmodes.c
 * Created: Sat Apr  7 22:27:05 2001 by tek@wiw.org
 * Revised: Sat Apr  7 22:33:14 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/raster.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int i, nmodes;
    d_rasterdescription_t *modes;

    d_raster_new();
    modes = d_raster_getmodes(&nmodes);
    for(i = 0; i < nmodes; i++)
        printf("%dx%dx%d\n", modes[i].w, modes[i].h, modes[i].bpp);
    d_raster_delete();
    return 0;
}

/* EOF listmodes.c */
