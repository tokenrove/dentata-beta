/* 
 * event_svgalib.c
 * Created: Thu Feb  1 21:20:12 2001 by tek@wiw.org
 * Revised: Sun Feb 25 01:22:29 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Event module for dentata gen beta SVGAlib port.
 */

#include <dentata/event.h>
#include <dentata/set.h>
#include <dentata/memory.h>

#include <vgakeyboard.h>

byte d_event_new(byte);
void d_event_delete(void);
bool d_event_map(byte, byte);
void d_event_unmap(byte);
bool d_event_ispressed(byte);

static void d_event_update(void);

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
    }

    evmap = d_memory_new(sizeof(d_set_t *)*D_EVENT_MAXEVENTS);
    for(i = 0; i < D_EVENT_MAXEVENTS; i++)
        evmap[i] = NULL;

    /* FIXME install event update handler here */

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

    /* FIXME remove event update handler here */

    return;
}

bool d_event_map(byte handle, byte event)
{
    if(evmap[handle] == NULL) evmap[handle] = d_set_new(SIZEHINT);
    return d_set_addelement(evmap[handle], event, NULL);
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

/* EOF event_svgalib.c */
