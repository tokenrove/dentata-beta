/* 
 * x11.c
 * Created: Mon Jan  8 05:12:00 2001 by tek@wiw.org
 * Revised: Thu Apr 12 01:47:18 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Dentata generation Beta
 * Raster module, X11 version
 */

#include <dentata/raster.h>
#include <dentata/memory.h>
#include <dentata/error.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

bool d_raster_new(void);
void d_raster_delete(void);
bool d_raster_setmode(d_rasterdescription_t);
d_rasterdescription_t *d_raster_getmodes(int *);
d_rasterdescription_t d_raster_getcurrentmode(void);
byte *d_raster_getgfxpointer(void);
void d_raster_update(void);

/* static variables */
static byte *vbuf;
enum { NAVAILMODES = 4 };
static d_rasterdescription_t availmodes[NAVAILMODES] = {
  { 320, 200, 0, 0, false, RGB },
  { 640, 480, 0, 0, false, RGB },
  { 800, 600, 0, 0, false, RGB },
  {1024, 768, 0, 0, false, RGB }
};
static d_rasterdescription_t curmode;
static XImage *xim;
static GC graphcont;
Display *_d_x11_display;
Window _d_x11_window;
static XShmSegmentInfo shminfo;
static int depth;

bool d_raster_new(void)
{
    Screen *screen;

    vbuf = NULL;
    _d_x11_window = 0;
    _d_x11_display = XOpenDisplay(NULL);
    if(_d_x11_display == NULL) {
        d_error_push("d_raster_new: Unable to open X11 display.");
        return failure;
    }

    screen = DefaultScreenOfDisplay(_d_x11_display);
    if(screen == NULL) {
        d_error_push("d_raster_new: DefaultScreenOfDisplay was NULL!");
        return failure;
    }
    graphcont = DefaultGCOfScreen(screen);
    depth = DefaultDepthOfScreen(screen);

    return success;
}

static void d_raster_closewindow(void)
{
    if(_d_x11_window) {
        XShmDetach(_d_x11_display, &shminfo);
        if(shminfo.shmaddr)
            shmdt(shminfo.shmaddr);
        if(shminfo.shmid >= 0)
            shmctl(shminfo.shmid, IPC_RMID, 0);
        XDestroyImage(xim);
        XDestroyWindow(_d_x11_display, _d_x11_window);
        XFlush(_d_x11_display);
        _d_x11_window = 0;
    }
    vbuf = NULL;
    return;
}

void d_raster_delete(void)
{
    d_raster_closewindow();
    XCloseDisplay(_d_x11_display);
    return;
}

bool d_raster_setmode(d_rasterdescription_t mode)
{
    Screen *screen;
    int i;

    for(i = 0; i < NAVAILMODES; i++) {
        if(mode.w      == availmodes[i].w &&
           mode.h      == availmodes[i].h &&
           mode.bpp    == depth &&
           mode.cspace == RGB) {
            curmode = mode;
            break;
        }
    }
    if(i == NAVAILMODES) {
        d_error_push("d_raster_setmode: no such mode available.");
        return failure;
    }

    d_raster_closewindow();

    screen = DefaultScreenOfDisplay(_d_x11_display);
    if(screen == NULL) {
        d_error_push("d_raster_setmode: DefaultScreenOfDisplay was NULL!");
        return failure;
    }

    _d_x11_window = XCreateSimpleWindow(_d_x11_display,
                                        RootWindowOfScreen(screen),
                                        0, 0, mode.w, mode.h, 1,
                                        WhitePixelOfScreen(screen),
                                        BlackPixelOfScreen(screen));
    if(_d_x11_window == 0) {
        d_error_push("d_raster_setmode: XCreateSimpleWindow returned 0.");
        return failure;
    }

    XStoreName(_d_x11_display, _d_x11_window, "dentata gen beta");

    xim = XShmCreateImage(_d_x11_display, DefaultVisualOfScreen(screen), depth,
                          ZPixmap, NULL, &shminfo, mode.w, mode.h);
    if(xim == NULL) {
        d_error_push("d_raster_setmode: XShmCreateImage failed for video "
                     "buffer.");
        return failure;
    }

    shminfo.shmid = shmget(IPC_PRIVATE, xim->bytes_per_line*xim->height,
                           IPC_CREAT|0777);
    if(shminfo.shmid < 0) {
        d_error_push("d_raster_setmode: shmget failed for video buffer "
                     "image.");
        return failure;
    }

    shminfo.shmaddr = shmat(shminfo.shmid, 0, 0);
    xim->data = shminfo.shmaddr;
    if(xim->data == NULL) {
        d_error_push("d_raster_setmode: shmat failed for video buffer image.");
        return failure;
    }
    
    shminfo.readOnly = False;
    if(XShmAttach(_d_x11_display, &shminfo) == 0) {
        d_error_push("d_raster_setmode: XShmAttach failed.");
        return failure;
    }
    XMapRaised(_d_x11_display, _d_x11_window);
    XClearWindow(_d_x11_display, _d_x11_window);
    XFlush(_d_x11_display);

    vbuf = (byte *)xim->data;
    return success;
}

d_rasterdescription_t *d_raster_getmodes(int *nmodes)
{
    d_rasterdescription_t *modes;
    int i;

    *nmodes = NAVAILMODES;
    modes = d_memory_new(*nmodes*sizeof(d_rasterdescription_t));
    if(modes == NULL) {
        d_error_push("d_raster_getmodes: failed to allocate memory for mode "
                     "list.");
        return NULL;
    }

    for(i = 0; i < NAVAILMODES; i++) {
        modes[i] = availmodes[i];
        modes[i].bpp = depth;
        if(depth == 8)
            modes[i].paletted = true;
    }

    return modes;
}

d_rasterdescription_t d_raster_getcurrentmode(void)
{
    return curmode;
}

byte *d_raster_getgfxpointer(void)
{
    return vbuf;
}

void d_raster_update(void)
{
    XShmPutImage(_d_x11_display, _d_x11_window, graphcont, xim, 0, 0, 0, 0,
                 curmode.w, curmode.h, False);
    XFlush(_d_x11_display);
    return;
}

void d_raster_setpalette(d_palette_t *p)
{
    d_error_fatal("d_raster_setpalette: Palette functions unimplemented!");
    return;
}

void d_raster_getpalette(d_palette_t *p)
{
    int i, pal[D_NCLUTITEMS*D_BYTESPERCOLOR];

    d_error_push("d_raster_getpalette: unimplemented.");
    for(i = 0; i < D_NCLUTITEMS; i++)
        p->clut[i] = pal[i];
    return;
}

/* EOF x11.c */
