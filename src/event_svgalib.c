/* 
 * svgalib.c
 * Created: Thu Feb  1 21:20:12 2001 by tek@wiw.org
 * Revised: Sat Feb 24 23:31:25 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/event.h>

#include <vgakeyboard.h>

byte d_event_new(byte);
void d_event_delete(void);
bool d_event_map(byte, byte);
void d_event_unmap(byte);
bool d_event_ispressed(byte);

static byte evmask;
static d_set_t *evmap[D_EVENT_MAXEVENTS];

byte d_event_new(byte mask)
{
    int i;

    evmask = 0;

    if(mask&D_EVENT_KEYBOARD) {
        if(keyboard_init() == 0) evmask |= D_EVENT_KEYBOARD;
    }

    if(mask&D_EVENT_MOUSE) {
    }

    for(i = 0; i < D_EVENT_MAXEVENTS; i++)
        evmap[i] = d_set_new();

    return evmask;
}

void d_event_delete(void)
{
    if(evmask&D_EVENT_KEYBOARD) {
        keyboard_close();
    }
    evmask = 0;

    return;
}

bool d_event_map(byte handle, byte event)
{
    return d_set_addelement(evmap[handle], event, NULL);
}

void d_event_unmap(byte handle)
{
    d_set_delete(evmap[handle]);
    evmap[handle] = d_set_new();
    return;
}

bool d_event_ispressed(byte handle)
{
    dword event;
    int i;

    d_set_resetiterator(evmap[handle]);
    while(event = d_set_nextkey(evmap[handle]), event != D_SET_INVALIDKEY) {
        if(event >= D_KBD_FIRST &&
           event <= D_KBD_LAST) {
            if(keyboard_ispressed(event)) {
                return true;
            }
        }
    }
    return false;
}

void d_event_update(void)
{
    if(evmask&D_EVENT_KEYBOARD) {
        keyboard_update();
    }

    return;
}

/* EOF svgalib.c */
