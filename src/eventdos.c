/* 
 * eventdos.c
 * Created: Mon Jun 25 21:38:03 2001 by tek@wiw.org
 * Revised: Mon Jun 25 21:47:24 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/event.h>
#include <dentata/set.h>
#include <dentata/memory.h>

byte d_event_new(byte mask);
void d_event_delete(void);
bool d_event_map(byte handle, byte event);
void d_event_unmap(byte handle);
bool d_event_ispressed(byte handle);
void d_event_update(void);

extern bool _d_event_kbdinit(void);
extern void _d_event_kbdclose(void);
extern bool _d_event_kbdishit(byte);

#define SIZEHINT 8

static d_set_t **evmap;
static byte evmask;

byte d_event_new(byte mask)
{
    int i;

    if(_d_event_kbdinit() == success)
        evmask = mask&D_EVENT_KEYBOARD;
    else
        evmask = 0;

    evmap = d_memory_new(D_EVENT_MAXEVENTS*sizeof(d_set_t *));
    for(i = 0; i < D_EVENT_MAXEVENTS; i++)
        evmap[i] = NULL;

    return evmask; 
}

void d_event_delete(void)
{
    int i;

    if(evmask&D_EVENT_KEYBOARD)
        _d_event_kbdclose();

    for(i = 0; i < D_EVENT_MAXEVENTS; i++)
        if(evmap[i] != NULL) d_set_delete(evmap[i]);
    d_memory_delete(evmap);
    return;
}

bool d_event_map(byte handle, byte event)
{
    if(evmap[handle] == NULL) {
        evmap[handle] = d_set_new(SIZEHINT);
        if(evmap[handle] == NULL) return failure;
    }
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

    d_set_resetiteration(evmap[handle]);
    while(event = d_set_nextkey(evmap[handle]), event != D_SET_INVALIDKEY) {
        if(event >= D_KBD_FIRST &&
           event <= D_KBD_LAST) {
            if(_d_event_kbdishit((byte)event)) {
                return true;
            }
        }
    }
    return false;
}

void d_event_update(void)
{
    return;
}

/* EOF eventdos.c */
