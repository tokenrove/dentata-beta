/* 
 * event_null.c
 * Created: Sun Feb 25 01:04:11 2001 by tek@wiw.org
 * Revised: Sun Feb 25 01:13:19 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Null implementation of the event module for testing/porting purposes.
 */

#include <dentata/event.h>
#include <dentata/set.h>
#include <dentata/memory.h>

byte d_event_new(byte mask);
void d_event_delete(void);
bool d_event_map(byte handle, byte event);
void d_event_unmap(byte handle);
bool d_event_ispressed(byte handle);

#define SIZEHINT 8

static d_set_t **evmap;
static byte evmask;

byte d_event_new(byte mask)
{
    int i;

    evmask = mask&0; /* no devices supported */

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
    return false;
}

/* EOF event_null.c */
