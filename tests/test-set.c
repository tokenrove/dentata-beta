/* 
 * test-set.c
 * Created: Sat Feb 24 23:52:05 2001 by tek@wiw.org
 * Revised: Sun Feb 25 00:24:22 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Note that much of the input/sequence of commands is _intentionally_
 * not verified, so that common error conditions can be tested, like
 * removing too many elements.
 *
 * Wishlist:
 *     - ability to fire out more random crap
 *     - testing multiple sets in parallel
 *     - testing seqs as well as sets
 *     - more thorough `c'heck command
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dentata/set.h>
#include <dentata/error.h>

void checkerr(bool verbose);
void usage(void);

void usage(void)
{
    printf("\
usage: test-set [options] <command> [commands...]

The following are valid options:
  -q                    Quiet (don't output status messages)
  -c                    Careful mode (abort on any call's failure, not just
                        when errors have been pushed or the check command
                        fails)

The following are valid commands:
  n[sizehint]           Creates a new set, with specified sizehint. (default 0)
  a[n]                  Adds n new elements to the set. (default 1)
  c                     Checks set integrity, exiting with EXIT_FAILURE if
                        something has gone wrong.
  r[n]                  Removes n arbitrary elements from the set. (default 1)
  d                     Deletes the set.
  i[n]                  Iterate through n elements from the set. (default:
                        the entire set)
");
    exit(EXIT_FAILURE);
    return;
}

int main(int argc, char **argv)
{
    int i, j, k;
    d_set_t *set;
    bool verbose = true, careful = false, ret;
    void *p;

    k = 0;

    for(i = 1; i < argc; i++) {
        switch(argv[i][0]) {
        case '-':
            switch(argv[i][1]) {
            case 'q':
                verbose = false;
                break;

            case 'c':
                careful = true;
                break;

            default:
                usage();
            }

        case 'n': /* new */
            if(isdigit(argv[i][1])) {
                argv[i]++;
                j = atoi(argv[i]);
            } else
                j = 0;

            set = d_set_new(j);
            checkerr(verbose);
            if(set == NULL) {
                if(verbose) fprintf(stderr, "test-set: new failed.\n");
                if(careful) exit(EXIT_FAILURE);
            }
            break;

        case 'd': /* delete */
            d_set_delete(set);
            checkerr(verbose);
            break;

        case 'a': /* add */
            if(isdigit(argv[i][1])) {
                argv[i]++;
                j = atoi(argv[i]);
            } else
                j = 1;

            ret = success;
            for(; j >= 0; j--) {
                ret &= d_set_add(set, k, NULL);
                k++;
            }
            checkerr(verbose);
            if(ret == failure) {
                if(verbose) fprintf(stderr, "test-set: add failed.\n");
                if(careful) exit(EXIT_FAILURE);
            }
            break;

        case 'r': /* remove */
            if(isdigit(argv[i][1])) {
                argv[i]++;
                j = atoi(argv[i]);
            } else
                j = 1;

            ret = success;
            for(; j >= 0; j--) {
                --k;
                ret &= d_set_add(set, k, NULL);
            }
            checkerr(verbose);
            if(ret == failure) {
                if(verbose) fprintf(stderr, "test-set: remove failed.\n");
                if(careful) exit(EXIT_FAILURE);
            }
            break;

        case 'c': /* check */
            ret = success;
            for(j = 0; j < k; j++) {
                p = &ret; /* set p to something arbitrary */
                ret &= d_set_fetch(set, j, (j%2) ? &p : NULL);
                if(j%2 && p != NULL) ret = failure;
            }

            if(ret != success) {
                if(verbose) fprintf(stderr, "test-set: check failed.\n");
                exit(EXIT_FAILURE);
            }
            break;

        default:
            usage();
        }
    }
    exit(EXIT_SUCCESS);
}

void checkerr(bool verbose)
{
    char *s;

    s = d_error_pop();
    if(s != NULL) {
        if(verbose) fprintf(stderr, "%s\n", s);
        exit(EXIT_FAILURE);
    }
    return;

}

/* EOF test-set.c */
