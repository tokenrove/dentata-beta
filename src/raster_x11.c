/* 
 * x11.c
 * Created: Mon Jan  8 05:12:00 2001 by tek@wiw.org
 * Revised: Sun Feb 25 05:07:14 2001 by tek@wiw.org
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
enum { NAVAILMODES = 6 };
static d_rasterdescription_t availmodes[NAVAILMODES] = {
  { 320, 200, 8,  0, true,  RGB },
  { 320, 200, 16, 0, false, RGB },
  { 320, 200, 24, 0, false, RGB },
  { 640, 480, 8,  0, true,  RGB },
  { 640, 480, 16, 0, false, RGB },
  { 640, 480, 24, 0, false, RGB }
};
static int curmode;
static XImage *xim;
static GC graphcont;
static Display *display;
static Window window;
static XShmSegmentInfo shminfo;

bool d_raster_new(void)
{
    vbuf = NULL;
    window = 0;
    display = XOpenDisplay(NULL);
    if(display == NULL) {
        d_error_push("d_raster_new: Unable to open X11 display.");
        return failure;
    }

    return success;
}

static void d_raster_closewindow(void)
{
    if(window) {
        XShmDetach(display, &shminfo);
        if(shminfo.shmaddr)
            shmdt(shminfo.shmaddr);
        if(shminfo.shmid >= 0)
            shmctl(shminfo.shmid, IPC_RMID, 0);
        if(xim->depth != availmodes[curmode].bpp &&
           vbuf != NULL)
            d_memory_delete(vbuf);
        XDestroyImage(xim);
        XDestroyWindow(display, window);
        XFlush(display);
        window = 0;
    }
    vbuf = NULL;
    return;
}

void d_raster_delete(void)
{
    d_raster_closewindow();
    XCloseDisplay(display);
    return;
}

bool d_raster_setmode(d_rasterdescription_t mode)
{
    Screen *screen;
    int i, depth;

    for(i = 0; i < NAVAILMODES; i++) {
        if(mode.w      == availmodes[i].w &&
           mode.h      == availmodes[i].h &&
           mode.bpp    == availmodes[i].bpp &&
           mode.cspace == availmodes[i].cspace) {
            curmode = i;
            break;
        }
    }
    if(i == NAVAILMODES) {
        d_error_push("d_raster_setmode: no such mode available.");
        return failure;
    }

    d_raster_closewindow();

    screen = DefaultScreenOfDisplay(display);
    if(screen == NULL) {
        d_error_push("d_raster_setmode: DefaultScreenOfDisplay was NULL!");
        return failure;
    }
    graphcont = DefaultGCOfScreen(screen);
    depth = DefaultDepthOfScreen(screen);
    if((depth == 8 && mode.bpp > 8) ||
       depth < 8) {
        d_error_push("d_raster_setmode: depths are too different to bother "
                     "with.");
        return failure;
    }

    window = XCreateSimpleWindow(display, RootWindowOfScreen(screen), 0, 0,
                                 mode.w, mode.h, 1, WhitePixelOfScreen(screen),
                                 BlackPixelOfScreen(screen));
    if(window == 0) {
        d_error_push("d_raster_setmode: XCreateSimpleWindow returned 0.");
        return failure;
    }

    XStoreName(display, window, "dentata gen beta");

    xim = XShmCreateImage(display, DefaultVisualOfScreen(screen), depth,
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
    if(XShmAttach(display, &shminfo) == 0) {
        d_error_push("d_raster_setmode: XShmAttach failed.");
        return failure;
    }
    XMapRaised(display, window);
    XClearWindow(display, window);
    XFlush(display);

    if(xim->depth == mode.bpp)
        vbuf = (byte *)xim->data;
    else {
        vbuf = d_memory_new((mode.w*mode.h*mode.bpp+7)/8);
        if(vbuf == NULL) {
            d_error_push("d_raster_setmode: allocation of vbuf failed.");
            return failure;
        }
    }

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

    for(i = 0; i < NAVAILMODES; i++)
        modes[i] = availmodes[i];

    return modes;
}

d_rasterdescription_t d_raster_getcurrentmode(void)
{
    return availmodes[curmode];
}

byte *d_raster_getgfxpointer(void)
{
    return vbuf;
}

void d_raster_update(void)
{
    if(xim->depth != availmodes[curmode].bpp) {
        /* do some conversion stuff */
    }

    XShmPutImage(display, window, graphcont, xim, 0, 0, 0, 0,
                 availmodes[curmode].w, availmodes[curmode].h, False);
    XFlush(display);
    return;
}


/* EOF x11.c */
