/* 
 * rastrx11.c
 * Created: Mon Jan  8 05:12:00 2001 by tek@wiw.org
 * Revised: Sat Jun 23 04:11:38 2001 by tek@wiw.org
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
byte *d_raster_getgfxptr(void);
void d_raster_update(void);

/* static variables */
static byte *vbuf;
enum { NAVAILMODES = 9 };
static d_rasterdescription_t availmodes[NAVAILMODES] = {
  { 240, 160, 0, 0, false, RGB },
  { 320, 200, 0, 0, false, RGB },
  { 320, 240, 0, 0, false, RGB },
  { 640, 400, 0, 0, false, RGB },
  { 640, 480, 0, 0, false, RGB },
  { 800, 600, 0, 0, false, RGB },
  {1024, 768, 0, 0, false, RGB },
  {1192, 864, 0, 0, false, RGB },
  {1280, 1024, 0, 0, false, RGB }
};
static d_rasterdescription_t curmode;
static XImage *xim;
static GC graphcont;
Display *_d_x11_display = NULL;
Window _d_x11_window;
static XShmSegmentInfo shminfo;
static int depth;
static bool useshm;
static Colormap cmap;
static XColor colors[D_NCLUTITEMS];

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
    useshm = (XShmQueryExtension(_d_x11_display) == True)?true:false;
    useshm = false; /* FIXME */

    return success;
}

static void d_raster_closewindow(void)
{
    if(_d_x11_window) {
        if(useshm) {
            XShmDetach(_d_x11_display, &shminfo);
            XDestroyImage(xim);
            shmdt(shminfo.shmaddr);
            shmctl(shminfo.shmid, IPC_RMID, 0);
        } else {
            d_memory_delete(xim->data);
            xim->data = NULL;
            XDestroyImage(xim);
        }
        XDestroyWindow(_d_x11_display, _d_x11_window);
        _d_x11_window = 0;
    }
    vbuf = NULL;
    return;
}

void d_raster_delete(void)
{
    d_raster_closewindow();
    XCloseDisplay(_d_x11_display);
    _d_x11_display = NULL;
    return;
}

bool d_raster_setmode(d_rasterdescription_t mode)
{
    Screen *screen;
    int i;

    for(i = 0; i < NAVAILMODES; i++) {
        if(mode.w      == availmodes[i].w &&
           mode.h      == availmodes[i].h &&
           mode.bpp    == depth && /* FIXME should check XListDepths */
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

    if(useshm)
        xim = XShmCreateImage(_d_x11_display, DefaultVisualOfScreen(screen),
                              mode.bpp, ZPixmap, NULL, &shminfo, mode.w,
                              mode.h);
    else
        xim = XCreateImage(_d_x11_display, DefaultVisualOfScreen(screen),
                           mode.bpp, ZPixmap, 0, NULL, mode.w, mode.h,
                           mode.bpp, 0);
    if(xim == NULL) {
        d_error_push("d_raster_setmode: Failed to create video buffer.");
        return failure;
    }

    if(useshm) {
        shminfo.shmid = shmget(IPC_PRIVATE, xim->bytes_per_line*xim->height,
                               IPC_CREAT|0777);
        if(shminfo.shmid < 0) {
            d_error_push("d_raster_setmode: shmget failed for video buffer "
                         "image.");
            return failure;
        }

        shminfo.shmaddr = shmat(shminfo.shmid, 0, 0);
        xim->data = shminfo.shmaddr;
    } else {
        xim->data = d_memory_new(xim->bytes_per_line*xim->height);
    }
    if(xim->data == NULL) {
        d_error_push("d_raster_setmode: Failed to create video buffer image.");
        return failure;
    }

    if(useshm) {
        shminfo.readOnly = False;
        if(XShmAttach(_d_x11_display, &shminfo) == 0) {
            d_error_push("d_raster_setmode: XShmAttach failed.");
            return failure;
        }
    }

    if(mode.paletted) {
        cmap = XCreateColormap(_d_x11_display, _d_x11_window,
                               DefaultVisualOfScreen(screen), True);
        XSetWindowColormap(_d_x11_display, _d_x11_window, cmap);
        for(i = 0; i < D_NCLUTITEMS; i++) {
            colors[i].red = colors[i].blue = colors[i].green = 0;
            colors[i].pixel = i; colors[i].flags = DoRed | DoGreen | DoBlue;
        }
        XStoreColors(_d_x11_display, cmap, colors, D_NCLUTITEMS);
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
    int i, j, ndepths, *depths;
    int scr = DefaultScreen(_d_x11_display);

    depths = XListDepths(_d_x11_display, scr, &ndepths);

    *nmodes = NAVAILMODES*ndepths;
    modes = d_memory_new(*nmodes*sizeof(d_rasterdescription_t));
    if(modes == NULL) {
        d_error_push("d_raster_getmodes: failed to allocate memory for mode "
                     "list.");
        return NULL;
    }

    for(i = 0, j = 0; i < NAVAILMODES*ndepths; i++) {
        if(availmodes[i/ndepths].w > DisplayWidth(_d_x11_display, scr) ||
           availmodes[i/ndepths].h > DisplayHeight(_d_x11_display, scr)) {
            continue;
        }
        modes[j] = availmodes[i/ndepths];
        modes[j].bpp = depths[i%ndepths];
        if(depths[i%ndepths] == 8)
            modes[j].paletted = true;
        j++;
    }
    XFree(depths);

    *nmodes = j;
    return modes;
}

d_rasterdescription_t d_raster_getcurrentmode(void)
{
    return curmode;
}

byte *d_raster_getgfxptr(void)
{
    return vbuf;
}

void d_raster_update(void)
{
    if(useshm)
        XShmPutImage(_d_x11_display, _d_x11_window, graphcont, xim, 0, 0, 0, 0,
                     curmode.w, curmode.h, False);
    else
        XPutImage(_d_x11_display, _d_x11_window, graphcont, xim, 0, 0, 0, 0,
                  curmode.w, curmode.h);
    XFlush(_d_x11_display);
    return;
}

void d_raster_setpalette(d_palette_t *p)
{
    int i;

    if(curmode.paletted) {
        for(i = 0; i < D_NCLUTITEMS; i++) {
            colors[i].red = p->clut[3*i+0];
            colors[i].green = p->clut[3*i+1];
            colors[i].blue = p->clut[3*i+2];
            colors[i].pixel = i;
            colors[i].flags = DoRed|DoGreen|DoBlue;
        }
        XStoreColors(_d_x11_display, cmap, colors, D_NCLUTITEMS);
    }
    return;
}

void d_raster_getpalette(d_palette_t *p)
{
    int i;

    for(i = 0; i < D_NCLUTITEMS; i++) {
        p->clut[3*i+0] = colors[i].red;
        p->clut[3*i+1] = colors[i].green;
        p->clut[3*i+2] = colors[i].blue;
    }
    return;
}

/* EOF rastrx11.c */
