/* 
 * event_x11.c
 * Created: Sun Feb 25 01:04:11 2001 by tek@wiw.org
 * Revised: Thu Apr 12 03:42:19 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * X11 implementation of the event module.
 */

#include <dentata/event.h>
#include <dentata/set.h>
#include <dentata/memory.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

byte d_event_new(byte mask);
void d_event_delete(void);
bool d_event_map(byte handle, byte event);
void d_event_unmap(byte handle);
bool d_event_ispressed(byte handle);
void d_event_update(void);

#define SIZEHINT 8

static d_set_t **evmap;
static d_set_t *events;
static byte evmask;

static int transtable[D_KBD_LAST+1] = {
    0, XK_Escape, XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7, XK_8, /* 9 */
    XK_9, XK_0, XK_minus, XK_equal, XK_BackSpace, XK_Tab, XK_Q, XK_W, /* 17 */
    XK_E, XK_R, XK_T, XK_Y, XK_U, XK_I, XK_O, XK_P, XK_bracketleft,
    XK_bracketright, XK_Return, XK_Control_L, /* 29 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 39 */
    0, 0, 0, 0, 0, XK_Z, XK_X, 0, 0, 0, /* 49 */
    0, 0, 0, 0, 0, 0, XK_Alt_L, XK_space, 0, 0, /* 59 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 69 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 79 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 89 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 99 */
    0, 0, 0, XK_Up, 0, XK_Left, XK_Right, 0, XK_Down, 0, /* 109 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 119 */
    0, 0, 0, 0, 0, 0, 0, 0        /* 127 */
};

extern Display *_d_x11_display;
extern Window _d_x11_window;

byte d_event_new(byte mask)
{
    int i;

    evmask = 0;

    if(mask&D_EVENT_KEYBOARD) {
        XSelectInput(_d_x11_display, _d_x11_window,
                     KeyPressMask|KeyReleaseMask);
        XAutoRepeatOff(_d_x11_display);
        evmask |= D_EVENT_KEYBOARD;
    }

    evmap = d_memory_new(D_EVENT_MAXEVENTS*sizeof(d_set_t *));
    for(i = 0; i < D_EVENT_MAXEVENTS; i++)
        evmap[i] = NULL;

    events = d_set_new(D_EVENT_MAXEVENTS);

    return evmask;
}

void d_event_delete(void)
{
    int i;

    d_set_delete(events);
    for(i = 0; i < D_EVENT_MAXEVENTS; i++)
        if(evmap[i] != NULL) d_set_delete(evmap[i]);
    d_memory_delete(evmap);
    return;
}

bool d_event_map(byte handle, byte event)
{
    if(handle >= D_EVENT_MAXEVENTS)
        return failure;

    if(evmap[handle] == NULL) {
        evmap[handle] = d_set_new(SIZEHINT);
        if(evmap[handle] == NULL) return failure;
    }
    return d_set_add(evmap[handle], event, NULL);
}

void d_event_unmap(byte handle)
{
    if(handle >= D_EVENT_MAXEVENTS) return;

    if(evmap[handle] != NULL) {
        d_set_delete(evmap[handle]);
        evmap[handle] = NULL;
    }
    return;
}

bool d_event_ispressed(byte handle)
{
    dword event;

    if(handle >= D_EVENT_MAXEVENTS) return false;
    if(evmap[handle] == NULL) return false;

    d_set_resetiteration(evmap[handle]);
    while(event = d_set_nextkey(evmap[handle]), event != D_SET_INVALIDKEY) {
        if(d_set_fetch(events, event, NULL)) {
            return true;
        }
    }
    return false;
}

void d_event_update(void)
{
    int i, j;
    XEvent event;

    while(XCheckWindowEvent(_d_x11_display, _d_x11_window,
                            KeyPressMask|KeyReleaseMask, &event) == True) {
        j = XLookupKeysym((XKeyEvent *)&event, 0);
        if(event.type == KeyPress) {
            for(i = 0; i < D_KBD_LAST; i++)
                if(transtable[i] == j) {
                    d_set_add(events, i, NULL);
                    break;
                }
        }
        else if(event.type == KeyRelease) {
            for(i = 0; i < D_KBD_LAST; i++)
                if(transtable[i] == j) {
                    d_set_remove(events, i);
                    break;
                }
        }
    }
    return;
}

/* EOF event_x11.c */
