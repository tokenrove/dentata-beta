/* 
 * svgalib.c
 * Created: Thu Feb  1 21:20:12 2001 by tek@wiw.org
 * Revised: Tue Feb  6 13:13:57 2001 by tek@wiw.org
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
        d_set_new(evmap[i], sizeof(byte));

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
    return d_set_addelement(evmap[handle], event);
}

void d_event_unmap(byte handle)
{
    d_set_delete(evmap[handle]);
    d_set_new(evmap[handle], sizeof(byte));
    return;
}

bool d_event_ispressed(byte handle)
{
    byte event;
    int i;

    d_set_resetiterator(evmap[handle]);
    for(i = d_set_nelements(evmap[handle]); i >= 0; i--) {
        event = *(byte *)d_set_nextelement(evmap[handle]);
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
