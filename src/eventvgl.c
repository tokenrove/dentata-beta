/* 
 * eventvgl.c
 * Created: Thu Feb  1 21:20:12 2001 by tek@wiw.org
 * Revised: Sat Jun 23 04:09:11 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Event module for dentata gen beta SVGAlib port.
 */

#include <dentata/types.h>
#include <dentata/event.h>
#include <dentata/set.h>
#include <dentata/memory.h>

#include <vga.h>
#include <vgakeyboard.h>
#include <vgamouse.h>

byte d_event_new(byte);
void d_event_delete(void);
bool d_event_map(byte, byte);
void d_event_unmap(byte);
bool d_event_ispressed(byte);
void d_event_update(void);

#define SIZEHINT 8

static d_set_t **evmap;
static byte evmask;

byte d_event_new(byte mask)
{
    int i;

    evmask = 0;

    if(mask&D_EVENT_KEYBOARD) {
        if(keyboard_init() == 0) evmask |= D_EVENT_KEYBOARD;
    }

    if(mask&D_EVENT_MOUSE) {
        vga_setmousesupport(1);
        if(vga_getmousetype() != MOUSE_NONE)
            evmask |= D_EVENT_MOUSE;
    }

    evmap = d_memory_new(sizeof(d_set_t *)*D_EVENT_MAXEVENTS);
    for(i = 0; i < D_EVENT_MAXEVENTS; i++)
        evmap[i] = NULL;

    return evmask;
}

void d_event_delete(void)
{
    int i;

    for(i = 0; i < D_EVENT_MAXEVENTS; i++)
        if(evmap[i] != NULL) d_set_delete(evmap[i]);
    d_memory_delete(evmap);

    if(evmask&D_EVENT_KEYBOARD) {
        keyboard_close();
    }
    evmask = 0;

    return;
}

bool d_event_map(byte handle, byte event)
{
    if(evmap[handle] == NULL) evmap[handle] = d_set_new(SIZEHINT);
    return d_set_add(evmap[handle], event, NULL);
}

void d_event_unmap(byte handle)
{
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
        if(event >= D_KBD_FIRST &&
           event <= D_KBD_LAST) {
            if(keyboard_keypressed(event)) {
                return true;
            }
        }

        if(event >= D_MOUSE_FIRST &&
           event <= D_MOUSE_LAST) {
            if(event == D_MOUSE_LEFT && mouse_getbutton()&MOUSE_LEFTBUTTON)
                return true;
            else if(event == D_MOUSE_RIGHT &&
                    mouse_getbutton()&MOUSE_RIGHTBUTTON)
                return true;
            else if(event == D_MOUSE_MIDDLE &&
                    mouse_getbutton()&MOUSE_MIDDLEBUTTON)
                return true;
        }
    }
    return false;
}

void d_event_update(void)
{
    if(evmask&D_EVENT_KEYBOARD) {
        keyboard_update();
    }
    if(evmask&D_EVENT_MOUSE) {
        mouse_update();
    }

    return;
}

/* EOF eventvgl.c */
