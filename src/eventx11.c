/* 
 * eventx11.c
 * Created: Sun Feb 25 01:04:11 2001 by tek@wiw.org
 * Revised: Wed Jul 18 23:11:05 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * X11 implementation of the event module.
 */

#include <dentata/types.h>
#include <dentata/event.h>
#include <dentata/set.h>
#include <dentata/memory.h>
#include <dentata/random.h>
#include <dentata/util.h>

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
    XK_A, XK_S, XK_D, XK_F, XK_G, XK_H, XK_J, XK_K, XK_L,
    XK_semicolon, /* 39 */
    XK_apostrophe, XK_grave, XK_Shift_L, XK_backslash, XK_Z, XK_X, XK_C,
    XK_V, XK_B, XK_N, /* 49 */
    XK_M, XK_comma, XK_period, XK_slash, XK_Shift_R, XK_asterisk, XK_Alt_L,
    XK_space, 0, 0, /* 59 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 69 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 79 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* 89 */
    0, 0, 0, 0, 0, 0, 0, XK_Control_R, XK_slash, XK_Sys_Req, /* 99 */
    XK_Alt_R, XK_Break, XK_Home, XK_Up, XK_Page_Up, XK_Left, XK_Right, XK_End,
    XK_Down, XK_Page_Down, /* 109 */
    XK_Insert, XK_Delete, 0, 0, 0, 0, 0, 0, 0, 0, /* 119 */
    0, 0, 0, 0, 0, 0, 0, 0        /* 127 */
};

extern Display *_d_x11_display;
extern Window _d_x11_window;

byte d_event_new(byte mask)
{
    int i;

    evmask = 0;

    if(_d_x11_display != NULL && mask&D_EVENT_KEYBOARD) {
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

    if(evmask&D_EVENT_KEYBOARD)
        XAutoRepeatOn(_d_x11_display);
    d_set_delete(events);
    for(i = 0; i < D_EVENT_MAXEVENTS; i++)
        if(evmap[i] != NULL) d_set_delete(evmap[i]);
    d_memory_delete(evmap);
    evmask = 0;
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
    d_iterator_t it;

    if(handle >= D_EVENT_MAXEVENTS) return false;
    if(evmap[handle] == NULL) return false;

    d_reset_iterator(&it);
    while(event = d_set_nextkey(&it, evmap[handle]),
          event != D_SET_INVALIDKEY) {
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

/* EOF eventx11.c */
