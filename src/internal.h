/* 
 * internal.h
 * Created: Sat Apr 28 16:29:17 2001 by tek@wiw.org
 * Revised: Sun Jun 24 03:10:42 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#define SETMAGIC 0xDEDE1240
#define SEQMAGIC 0xDEDE1241
#define S3MMAGIC 0xDEDE5333

extern void __d_image_clip(d_image_t *, d_image_t *, d_point_t, dword *,
                           dword *, dword *, dword *, dword *, dword *);
extern void _blit80(byte *, byte *, byte *, byte *, dword, dword, dword,
                    dword, dword, dword);
extern void _blit81(byte *, byte *, byte *, byte *, dword, dword, dword,
                    dword, dword, dword);
extern void _blit160(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit161(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit162(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit164(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit168(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit240(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit241(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit242(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit244(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);
extern void _blit248(byte *, byte *, byte *, byte *, dword, dword, dword,
                     dword, dword, dword);

/* EOF internal.h */
