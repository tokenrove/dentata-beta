/* 
 * colligen.c
 * Created: Tue May  1 06:59:14 2001 by tek@wiw.org
 * Revised: Fri Jul 13 04:58:50 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>

bool __collide_bounding(byte *a, byte *b, dword aoff, dword ascan,
                        dword scanlen, dword endoff, dword boff, dword bscan)
{
    /* If this got called, at least some pixels must match */
    return true;
}

bool __collide_pixel1(byte *a, byte *b, dword aoff, dword ascan,
                      dword scanlen, dword endoff, dword boff, dword bscan)
{
    byte *end;
    byte j, k;
    dword i;

    end = a+endoff/8;
    a += aoff/8;
    b += boff/8;
    j = aoff%8;
    k = boff%8;
    do {
        for(i = scanlen; i > 0; i--) {
            if(((*a)&(1<<j)) &&
               ((*b)&(1<<k)))
                return true;
            j++; k++;
            if(j == 8) {
                j = 0;
                a++;
            }
            if(k == 8) {
                k = 0;
                b++;
            }
        }
        a += ascan/8;
        b += bscan/8;
        j += ascan%8;
        k += bscan%8;
        if(j > 7) {
            a++;
            j %= 8;
        }
        if(k > 7) {
            b++;
            k %= 8;
        }
    } while(a < end);
    return false;
}

bool __collide_pixel2(byte *a, byte *b, dword aoff, dword ascan,
                      dword scanlen, dword endoff, dword boff, dword bscan)
{
    byte *end;
    byte j, k;
    dword i;

    end = a+endoff/4;
    a += aoff/4;
    b += boff/4;
    j = aoff%4;
    k = boff%4;
    do {
        for(i = scanlen; i > 0; i--) {
            if((*a&(3<<(2*j)))>>(2*j) == 3 &&
               (*b&(3<<(2*k)))>>(2*k) == 3) {
                return true;
            }
            j++; k++;
            if(j == 4) {
                j = 0;
                a++;
            }
            if(k == 4) {
                k = 0;
                b++;
            }
        }
        a += ascan/4;
        b += bscan/4;
        j += ascan%4;
        k += bscan%4;
        if(j > 3) {
            a++;
            j %= 4;
        }
        if(k > 3) {
            b++;
            k %= 4;
        }
    } while(a < end);
    return false;
}

bool __collide_pixel4(byte *a, byte *b, dword aoff, dword ascan,
                      dword scanlen, dword endoff, dword boff, dword bscan)
{
    byte *end;
    byte j, k;
    dword i;

    end = a+endoff/2;
    a += aoff/2;
    b += boff/2;
    j = aoff%2;
    k = boff%2;
    do {
        for(i = scanlen; i > 0; i--) {
            if((*a&(15<<(4*j)))>>(4*j) == 15 &&
               (*b&(15<<(4*k)))>>(4*k) == 15) {
                return true;
            }
            j++; k++;
            if(j == 2) {
                j = 0;
                a++;
            }
            if(k == 2) {
                k = 0;
                b++;
            }
        }
        a += ascan/2;
        b += bscan/2;
        j += ascan%2;
        k += bscan%2;
        if(j > 1) {
            a++;
            j %= 2;
        }
        if(k > 1) {
            b++;
            k %= 2;
        }
    } while(a < end);
    return false;
}

bool __collide_pixel8(byte *a, byte *b, dword aoff, dword ascan,
                      dword scanlen, dword endoff, dword boff, dword bscan)
{
    byte *end;
    int i;

    end = a+endoff;
    a += aoff;
    b += boff;
    do {
        for(i = scanlen; i > 0; i--) {
            if(*a == 255 && *b == 255)
                return true;
            a++; b++;
        }
        a += ascan;
        b += bscan;
    } while(a < end);
    return false;
}

bool __collide_8a1(byte *a, byte *b, dword aoff, dword ascan,
                   dword scanlen, dword endoff, dword boff, dword bscan)
{
    byte *end;
    int i;

    end = a+endoff;
    a += aoff;
    b += boff;
    do {
        for(i = scanlen; i > 0; i--) {
            if(*a != 0 && *b != 0)
                return true;
            a++; b++;
        }
        a += ascan;
        b += bscan;
    } while(a < end);
    return false;
}

bool __collide_thresh2(byte *a, byte *b, dword aoff, dword ascan,
                       dword scanlen, dword endoff, dword boff, dword bscan)
{
    byte *end;
    byte j, k;
    dword i;

    end = a+endoff/4;
    a += aoff/4;
    b += boff/4;
    j = aoff%4;
    k = boff%4;
    do {
        for(i = scanlen; i > 0; i--) {
            if(((*a&(3<<(2*j)))>>(2*j))+((*b&(3<<(2*k)))>>(2*k)) >= 3)
                return true;
            j++; k++;
            if(j == 4) {
                j = 0;
                a++;
            }
            if(k == 4) {
                k = 0;
                b++;
            }
        }
        a += ascan/4;
        b += bscan/4;
        j += ascan%4;
        k += bscan%4;
        if(j > 3) {
            a++;
            j %= 4;
        }
        if(k > 3) {
            b++;
            k %= 4;
        }
    } while(a < end);
    return false;
}

bool __collide_thresh4(byte *a, byte *b, dword aoff, dword ascan,
                       dword scanlen, dword endoff, dword boff, dword bscan)
{
    byte *end;
    byte j, k;
    dword i;

    end = a+endoff/2;
    a += aoff/2;
    b += boff/2;
    j = aoff%2;
    k = boff%2;
    do {
        for(i = scanlen; i > 0; i--) {
            if(((*a&(15<<(4*j)))>>(4*j))+((*b&(15<<(4*k)))>>(4*k)) >= 15)
                return true;
            j++; k++;
            if(j == 2) {
                j = 0;
                a++;
            }
            if(k == 2) {
                k = 0;
                b++;
            }
        }
        a += ascan/2;
        b += bscan/2;
        j += ascan%2;
        k += bscan%2;
        if(j > 1) {
            a++;
            j %= 2;
        }
        if(k > 1) {
            b++;
            k %= 2;
        }
    } while(a < end);
    return false;
}

bool __collide_thresh8(byte *a, byte *b, dword aoff, dword ascan,
                       dword scanlen, dword endoff, dword boff, dword bscan)
{
    byte *end;
    int i;

    end = a+endoff;
    a += aoff;
    b += boff;
    do {
        for(i = scanlen; i > 0; i--) {
            if((word)*a+(word)*b >= 255)
                return true;
            a++; b++;
        }
        a += ascan;
        b += bscan;
    } while(a < end);
    return false;
}

/* EOF colligen.c */
