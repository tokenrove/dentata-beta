/* 
 * eventgba.c
 * Created: Sun Jun 24 23:31:03 2001 by tek@wiw.org
 * Revised: Sun Jun 24 23:40:12 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 * 
 */

#include <dentata/types.h>
#include <dentata/event.h>
#include <dentata/set.h>
#include <dentata/memory.h>
#include <dentata/random.h>
#include <dentata/util.h>

byte d_event_new(byte mask);
void d_event_delete(void);
bool d_event_map(byte handle, byte event);
void d_event_unmap(byte handle);
bool d_event_ispressed(byte handle);
void d_event_update(void);

#define SIZEHINT 8

static d_set_t **evmap;
static byte evmask;

byte d_event_new(byte mask)
{
    int i;

    evmask = mask&D_EVENT_CONTROLLER;

    evmap = d_memory_new(D_EVENT_MAXEVENTS*sizeof(d_set_t *));
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
    word key;
    d_iterator_t it;

    d_iterator_reset(&it);
    key = *(word *)0x4000098; /* KEY register */
    while(event = d_set_nextkey(&it, evmap[handle]),
          event != D_SET_INVALIDKEY) {
        switch(event) {
        case D_CNTRLR_TRIGGERR:
            if(key&512) return true;
            break;
        case D_CNTRLR_TRIGGERL:
            if(key&256) return true;
            break;
        case D_CNTRLR_DOWN:
            if(key&128) return true;
            break;
        case D_CNTRLR_UP:
            if(key&64) return true;
            break;
        case D_CNTRLR_LEFT:
            if(key&32) return true;
            break;
        case D_CNTRLR_RIGHT:
            if(key&16) return true;
            break;
        case D_CNTRLR_START:
            if(key&8) return true;
            break;
        case D_CNTRLR_SELECT:
            if(key&4) return true;
            break;
        case D_CNTRLR_BUTTONB:
            if(key&2) return true;
            break;
        case D_CNTRLR_BUTTONA:
            if(key&1) return true;
            break;
        default:
            continue;
        }
    }
    return false;
}

void d_event_update(void)
{
    return;
}

/* EOF eventgba.c */
