/* 
 * blit_generic.c
 * Created: Mon Jan 29 13:42:41 2001 by tek@wiw.org
 * Revised: Thu Apr 12 01:45:12 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/blit.h>
#include <dentata/types.h>
#include <dentata/memory.h>

/*
 * 8 bpp
 */

void _blit80(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
             dword dscanoff, dword scanlen, dword endoffs, dword soffs,
             dword sscanoff);

void _blit80(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
             dword dscanoff, dword scanlen, dword endoffs, dword soffs,
             dword sscanoff)
{
    byte *end;

    end = ddat+endoffs;
    ddat += doffs;
    sdat += soffs;
    do {
        d_memory_copy(ddat, sdat, scanlen);
        ddat += scanlen+dscanoff;
        sdat += scanlen+sscanoff;
    } while(ddat < end);
    return;
}
   
/*
 * 16 bpp
 */

void _blit160(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff);
void _blit161(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff);
void _blit162(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff);
void _blit164(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff);
void _blit168(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff);

void _blit160(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff)
{
    register int i;
    byte *end;

    end = ddat+endoffs*2;
    ddat += doffs*2;
    sdat += soffs*2;
    do {
        for(i = scanlen; i > 0; i--) {
            *(ddat++) = *(sdat++);
            *(ddat++) = *(sdat++);
        }
        ddat += dscanoff*2;
        sdat += sscanoff*2;
    } while(ddat < end);
    return;
}

void _blit161(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff)
{
    register dword i;
    register byte j;
    byte *end;

    end = ddat+endoffs*2;
    ddat += doffs*2;
    sdat += soffs*2;

    j = soffs%8;
    salp += soffs/8;

    do {
        for(i = scanlen; i > 0; i--) {
            if(*salp&(1<<j)) {
                *(ddat++) = *(sdat++);
                *(ddat++) = *(sdat++);
            } else {
                ddat += 2;
                sdat += 2;
            }
            j++;
            if(j == 8) {
                j = 0;
                salp++;
            }
        }
        ddat += dscanoff*2;
        sdat += sscanoff*2;
        salp += sscanoff/8;
        j = (j+sscanoff)%8;
    } while(ddat < end);
    return;
}

void _blit162(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff)
{
    register dword i;
    register byte j, k, l, m;
    byte *end;
    byte masktable[4] = { 3, 15, 63, 255 };

    end = ddat+endoffs*2;
    ddat += doffs*2;
    sdat += soffs*2;

    j = soffs%4;
    salp += soffs/4;

    do {
        for(i = scanlen; i > 0; i--) {
            switch((*salp&masktable[j])>>(2*j)) {
            case 0:
                ddat += 2;
                sdat += 2;
                break;

            case 1:
                k = *sdat; l = *ddat; m = *sdat; sdat++; ddat++;
                k = ((k&31)/3+((l&31)*2)/3);
                l = (((m>>5)|((*sdat)<<3))/3)+
                    (((l>>5)|((*ddat)<<3))*2)/3;
                m = ((*sdat)>>3)/3+(((*ddat)>>3)*2)/3;
                *(ddat-1) = k|((l&7)<<5);
                *(ddat++) = ((l>>3)&7)|(m<<3);
                break;

            case 2:
                k = *sdat; l = *ddat; m = *sdat; sdat++; ddat++;
                k = (((k&31)*2)/3+(l&31)/3);
                l = (((m>>5)|((*sdat)<<3))*2)/3+
                    ((l>>5)|((*ddat)<<3))/3;
                m = (((*sdat)>>3)*2)/3+((*ddat)>>3)/3;
                *(ddat-1) = k|((l&7)<<5);
                *(ddat++) = ((l>>3)&7)|(m<<3);
                break;

            case 3:
                *(ddat++) = *(sdat++);
                *(ddat++) = *(sdat++);
                break;
            }
            j++;
            if(j == 4) {
                j = 0;
                salp++;
            }
        }
        ddat += dscanoff*2;
        sdat += sscanoff*2;
        salp += sscanoff/4;
        j = (j+sscanoff)%4;
    } while(ddat < end);
    return;
}

/* EOF blit_generic.c */
