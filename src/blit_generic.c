/* 
 * blit_generic.c
 * Created: Mon Jan 29 13:42:41 2001 by tek@wiw.org
 * Revised: Fri Apr 13 03:47:47 2001 by tek@wiw.org
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
   
void _blit81(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
             dword dscanoff, dword scanlen, dword endoffs, dword soffs,
             dword sscanoff)
{
    register dword i;
    register byte j;
    byte *end;

    end = ddat+endoffs;
    ddat += doffs;
    sdat += soffs;

    j = soffs%8;
    salp += soffs/8;

    do {
        for(i = scanlen; i > 0; i--) {
            if((*salp)&(1<<j)) {
                *(ddat++) = *(sdat++);
            } else {
                ddat++;
                sdat++;
            }
            j++;
            if(j == 8) {
                j = 0;
                salp++;
            }
        }
        ddat += dscanoff;
        sdat += sscanoff;
        salp += sscanoff/8;
        j = (j+sscanoff)%8;
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
            if((*salp)&(1<<j)) {
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
        j += sscanoff%8;
        if(j > 7) {
            salp++;
            j %= 8;
        }
    } while(ddat < end);
    return;
}

void _blit162(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff)
{
    register dword i;
    register byte j;
    register word k, l;
    byte *end;
    byte masktable[4] = { 3, 15, 63, 255 };

    end = ddat+endoffs*2;
    ddat += doffs*2;
    sdat += soffs*2;

    j = soffs%4;
    salp += soffs/4;

    do {
        for(i = scanlen; i > 0; i--) {
            switch(((*salp)&masktable[j])>>(2*j)) {
            case 0:
                ddat += 2;
                sdat += 2;
                break;

            case 1:
                k = (*sdat)&31;
                k = k/3;
                k += ((*ddat)&31)*2/3;
                l = (*(sdat++))>>5; l |= ((*sdat)&7)<<3;
                l = l/3;
                l += (((*ddat)>>5)|(((*(ddat+1))&7)<<3))*2/3;
                *(ddat++) = k|((l&7)<<5);
                k = (*(sdat++))>>3;
                k = k/3;
                k += (((*ddat)>>3)*170)*2/3;
                *(ddat++) = (k<<3)|(l>>3);
                break;

            case 2:
                k = (*sdat)&31;
                k = (k*2)/3;
                k += ((*ddat)&31)/3;
                l = (*(sdat++))>>5; l |= ((*sdat)&7)<<3;
                l = (l*2)/3;
                l += (((*ddat)>>5)|(((*(ddat+1))&7)<<3))/3;
                *(ddat++) = k|((l&7)<<5);
                k = (*(sdat++))>>3;
                k = (k*2)/3;
                k += (((*ddat)>>3)*170)/3;
                *(ddat++) = (k<<3)|(l>>3);
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
        j += sscanoff%4;
        if(j > 3) {
            salp++;
            j %= 4;
        }
    } while(ddat < end);
    return;
}

void _blit164(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff)
{
    register int i;
    register word j, k;
    register byte l, a;
    byte *end;

    end = ddat+endoffs*2;
    ddat += doffs*2;
    sdat += soffs*2;
    salp += soffs/2;
    l = soffs%2;

    do {
        for(i = scanlen; i > 0; i--) {
            if(l == 0)
                a = (*salp)&0x0f;
            else
                a = (*(salp++))>>4;

            j = (*sdat)&31;
            j = j*a/15;
            j += ((*ddat)&31)*(15-a)/15;
            k = (*(sdat++))>>5; k |= ((*sdat)&7)<<3;
            k = k*a/15;
            k += (((*ddat)>>5)|(((*(ddat+1))&7)<<3))*(15-a)/15;
            *(ddat++) = j|((k&7)<<5);
            j = (*(sdat++))>>3;
            j = j*a/15;
            j += ((*ddat)>>3)*(15-a)/15;
            *(ddat++) = (j<<3)|(k>>3);

            l = (l+1)%2;
        }
        ddat += dscanoff*2;
        sdat += sscanoff*2;
        salp += sscanoff/2;
        l += sscanoff%2;
        if(l > 1) {
            l = 0;
            salp++;
        }
    } while(ddat < end);
    return;
}

void _blit168(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff)
{
    register int i;
    register word j, k;
    byte *end;

    end = ddat+endoffs*2;
    ddat += doffs*2;
    sdat += soffs*2;
    salp += soffs;

    do {
        for(i = scanlen; i > 0; i--) {
            j = (*sdat)&31;
            j = j*(*salp)/255;
            j += ((*ddat)&31)*(255-(*salp))/255;
            k = (*(sdat++))>>5; k |= ((*sdat)&7)<<3;
            k = k*(*salp)/255;
            k += (((*ddat)>>5)|(((*(ddat+1))&7)<<3))*(255-(*salp))/255;
            *(ddat++) = j|((k&7)<<5);
            j = (*(sdat++))>>3;
            j = j*(*salp)/255;
            j += ((*ddat)>>3)*(255-(*(salp++)))/255;
            *(ddat++) = (j<<3)|(k>>3);
        }
        ddat += dscanoff*2;
        sdat += sscanoff*2;
        salp += sscanoff;
    } while(ddat < end);
    return;
}

/*
 * 24 bpp
 */

void _blit240(byte *ddat, byte *dalp, byte *sdat, byte *salp, dword doffs,
              dword dscanoff, dword scanlen, dword endoffs, dword soffs,
              dword sscanoff)
{
    register dword i;
    register byte j;
    byte *end;

    end = ddat+endoffs*3;
    ddat += doffs*3;
    sdat += soffs*3;

    do {
        for(i = scanlen; i > 0; i--) {
            *(ddat++) = *(sdat++);
            *(ddat++) = *(sdat++);
            *(ddat++) = *(sdat++);
        }
        ddat += dscanoff*3;
        sdat += sscanoff*3;
    } while(ddat < end);
    return;
}

/* EOF blit_generic.c */
