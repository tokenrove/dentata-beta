/* 
 * util.c
 * Created: Thu Apr 19 06:36:50 2001 by tek@wiw.org
 * Revised: Tue Jun 26 00:28:02 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * gen beta utility functions
 *
 */

#include <dentata/types.h>
#include <dentata/random.h>
#include <dentata/util.h>

d_point_t d_point_center(d_rect_t r);
d_point_t d_point_origin(void);
d_point_t d_point_random(d_random_t *rand, d_rect_t rect);
void d_iterator_reset(d_iterator_t *it);
int d_util_printflen(byte *fmt, void **args);
void d_util_sprintf(byte *s, byte *fmt, void **args);


d_point_t d_point_center(d_rect_t r)
{
    d_point_t pt;

    pt.x = r.w/2+r.offset.x;
    pt.y = r.h/2+r.offset.y;
    return pt;
}


d_point_t d_point_origin(void)
{
    d_point_t pt = { 0, 0 };

    return pt;
}


d_point_t d_point_random(d_random_t *rand, d_rect_t rect)
{
    d_point_t pt;

    pt.x = d_random_gen(rand, rect.w)+rect.offset.x;
    pt.y = d_random_gen(rand, rect.h)+rect.offset.y;

    return pt;
}


void d_iterator_reset(d_iterator_t *it)
{
    it->signature = 0;
    it->pos = 0;
    it->aux = NULL;
    return;
}


int d_util_printflen(byte *fmt, void **args)
{
    int i, j, integer;
    char *string;

    for(i = 0, j = 0; fmt[i]; i++) {
        if(fmt[i] == '%') {
            i++;
            switch(fmt[i]) {
            case 'd':
            case 'i':
                integer = *(int *)args;
		if(integer < 0) j++;
                do j++; while(integer/=10);
                args++;
                break;

            case 'o':
                integer = *(int *)args;
		if(integer < 0) j++;
                do j++; while(integer/=8);
                args++;
                break;

            case 'X':
            case 'x':
                integer = *(int *)args;
		if(integer < 0) j++;
                do j++; while(integer/=16);
                args++;
                break;

            case 's':
                string = *(char **)args;
                while(*(string++)) j++;
                args++;
                break;

            case 'c':
                string = (char *)args;
                args++;
                j++;
                break;

            default:
                j++;
            }
        } else {
            j++;
        }
    }

    return j;
}


void d_util_sprintf(byte *s, byte *fmt, void **args)
{
    int i, k, integer, width;
    char *string;

    for(i = 0; fmt[i]; i++) {
        if(fmt[i] == '%') {
            i++;
            switch(fmt[i]) {
            case 'd':
            case 'i':
                integer = *(int *)args;
		if(integer < 0) {
		    *s++ = '-';
		    integer = -integer;
		}
		k = integer;
                width = 1;
                while(k/=10) width++;
                s+=width-1;
                do {
                    *(s--) = '0'+(integer%10);
                    integer /= 10;
                } while(integer);
                s+=width+1;
                args++;
                break;

            case 'o':
                integer = *(int *)args;
		if(integer < 0) {
		    *s++ = '-';
		    integer = -integer;
		}
		k = integer;
                width = 1;
                do width++; while(k/=8);
                s+=width-1;
                do {
                    *(s--) = '0'+(integer%8);
                    integer /= 8;
                } while(integer);
                s+=width+1;
                args++;
                break;

            case 'X':
            case 'x':
                integer = *(int *)args;
		if(integer < 0) {
		    *s++ = '-';
		    integer = -integer;
		}
		k = integer;
                width = 1;
                while(k/=16) width++;
                s+=width-1;
                do {
                    if(integer%16 > 9)
                        *(s--) = ((fmt[i]=='x')?'a':'A')+(integer%16-10);
                    else
                        *(s--) = '0'+(integer%16);
                    integer /= 16;
                } while(integer);
                s+=width+1;
                args++;
                break;

            case 's':
                string = *(char **)args;
                while(*string) *(s++) = *(string++);
                args++;
                break;

            case 'c':
                *(s++) = *(char *)args;
                args++;
                break;

            default:
                *(s++) = fmt[i];
            }
        } else {
            *(s++) = fmt[i];
        }
    }
    *(s++) = '\0';

    return;
}

/* EOF util.c */
