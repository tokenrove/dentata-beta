/* 
 * fontcnvt.c
 * Created: Fri Apr 13 23:14:18 2001 by tek@wiw.org
 * Revised: Sat Jun 23 04:15:58 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGNAME "fontcnvt"

void usage(void)
{
    printf("usage:\n %s [-w <width>] [-h <height>] <in file> [out file]\n",
           PROGNAME);
    return;
}

int main(int argc, char **argv)
{
    FILE *in, *out;
    char *outname = NULL, *inname = NULL;
    unsigned short w, h;
    int i, j;

    w = 8;
    h = 8;

    for(i = 1, j = 0; i < argc; i++) {
        if(argv[i][0] == '-') {
            switch(argv[i][1]) {
            case 'w':
                i++;
                if(i >= argc) {
                    usage();
                    exit(EXIT_FAILURE);
                }
                w = atoi(argv[i]);
                if(w != 8) {
                    fprintf(stderr, "Warning: v0 fonts don't like width not "
                            "being 8.\n");
                }
                break;

            case 'h':
                i++;
                if(i >= argc) {
                    usage();
                    exit(EXIT_FAILURE);
                }
                h = atoi(argv[i]);
                break;

            default:
                usage();
                exit(EXIT_FAILURE);
            }
        } else {
            switch(j++) {
            case 0:
                inname = argv[i];
                break;
            case 1:
                outname = argv[i];
                break;
            case 2:
                usage();
                exit(EXIT_FAILURE);
            }
        }
    }

    if(inname == NULL) {
        usage();
        exit(EXIT_FAILURE);
    }
    if(outname == NULL) {
        malloc(strlen(inname)+1+strlen(".fnt"));
        strcpy(outname, inname);
        strcat(outname, ".fnt");
    }

    out = fopen(outname, "wb");
    if(out == NULL) {
        fprintf(stderr, "Failed to open ``%s'' for writing.\n", outname);
        exit(EXIT_FAILURE);
    }

    fwrite("FNT", 1, 3, out);
    fputc(0, out); /* version 0 */
    fputc(w&0xFF, out);
    fputc(w>>8, out);
    fputc(h&0xFF, out);
    fputc(h>>8, out);
    fputc(0, out);
    fputc(255, out);

    in = fopen(inname, "rb");
    if(in == NULL) {
        fprintf(stderr, "Failed to open ``%s'' for reading.\n", inname);
        fclose(out);
        remove(outname);
        exit(EXIT_FAILURE);
    }

    while(!feof(in)) {
        fputc(fgetc(in), out); /* not exactly the most efficient
                                  implementation */
    }
    if((ftell(in)-1)/w != 256*h) {
        fprintf(stderr, "Got %d instead of %d.\n", (ftell(in)-1)/w, 256*h);
    }

    fclose(in);
    fclose(out);

    exit(EXIT_SUCCESS);
}

/* EOF fontcnvt.c */
