/**
 * set.h
 * Created: Sat Feb 24 20:01:57 2001 by tek@wiw.org
 * Revised: Sat Jun 23 03:28:34 2001 by tek@wiw.org
 * Copyright 2001 Julian E. C. Squires (tek@wiw.org)
 * This program comes with ABSOLUTELY NO WARRANTY.
 * $Id$
 *
 * Module: set
 *
 * Include dentata/types.h before this file.
 */

#ifndef DENTATA_SET_H
#define DENTATA_SET_H

typedef void d_set_t;

#define D_SET_INVALIDKEY 0xFFFFFFFF

/**
 * d_set_new(int sizehint)
 * Creates a new set.
 *
 * Takes: sizehint - an estimate as to how many elements are going to be
 *                   in this set at any one time. Set to 0 to use internal
 *                   defaults which should be sane for most data.
 *
 * Returns: a pointer to the newly created set object. NULL on failure.
 */
extern d_set_t *d_set_new(int);

/**
 * d_set_delete(d_set_t *p)
 * Deletes a set and its elements.
 *
 * Takes: p - pointer to the set, previously created by d_set_new().
 *
 * Pushes an error if p is not valid.
 */
extern void d_set_delete(d_set_t *);

/**
 * d_set_add(d_set_t *p, dword key, void *data)
 * Adds an element and its satellite data to a set.
 *
 * Takes: p - pointer to the set.
 *        key - the key for this element.
 *        data - the data associated with this element.
 *
 * Returns: success if the element was added, failure if that element is
 *          already present in the set or if the key is D_SET_INVALIDKEY.
 *
 * Pushes an error if p is not valid.
 */
extern bool d_set_add(d_set_t *, dword, void *);

/**
 * d_set_remove(d_set_t *p, dword key)
 * Removes an element and its satellite data from a set.
 *
 * Takes: p - pointer to the set.
 *        key - the key for this element.
 *
 * Returns: success if the element was removed, failure if that element was
 *          not present in the set.
 *
 * Pushes an error if p is not valid.
 */
extern bool d_set_remove(d_set_t *, dword);

/**
 * d_set_fetch(d_set_t *p, dword key, void **data)
 * Retrieves an element from a set.
 *
 * Takes: p - pointer to the set.
 *        key - the key of the element desired.
 *        data - location where the satellite data pointer will be stored, if
 *               the element is found. If data is NULL, the satellite data
 *               will not be stored.
 *
 * Returns: true if the element is found, false otherwise.
 *
 * Pushes an error if p is not valid.
 */
extern bool d_set_fetch(d_set_t *, dword, void **);

/**
 * d_set_getunusedkey(d_set_t *p)
 * Gets an unused key. In hash table implementations of this module,
 * this is prefered over just randomly selecting key values, as it will
 * attempt to return key values which do not collide with others.
 *
 * Takes: p - pointer to the set.
 *
 * Returns: an unused key value, or D_SET_INVALIDKEY if there are no
 *          more valid keys possible in this set.
 *
 * Pushes an error if p is not valid or if there are no more valid keys
 * possible in this set. (Something has usually gone horribly wrong if
 * this is the case -- the keyspace should allow a little more than
 * 4 billion keys)
 */
extern dword d_set_getunusedkey(d_set_t *p);

/**
 * d_set_nelements(d_set_t *p)
 * Gets the number of elements present in the set.
 *
 * Takes: p - pointer to the set.
 *
 * Returns: the number of elements in the set.
 *
 * Pushes an error if p is not valid.
 */
extern int d_set_nelements(d_set_t *);

/**
 * d_set_resetiteration(d_set_t *p)
 * Resets the internal iteration counter. Note that this should be called
 * before any calls to d_set_isiterationcomplete() or d_set_nextkey() if
 * the set has been changed in any way since the last time it was called.
 *
 * Takes: p - pointer to the set.
 *
 * Pushes an error if p is not valid.
 */
extern void d_set_resetiteration(d_set_t *);

/**
 * d_set_nextkey(d_set_t *p)
 * Gets the key for the next element in the iteration.
 *
 * Takes: p - pointer to the set.
 *
 * Returns: the key for the next element in the iteration, or D_SET_INVALIDKEY
 *          if the iteration has reached completition or the iteration was not
 *          reset with d_set_resetiteration() after changes had been made to
 *          the set.
 *
 * Pushes an error if p is not valid or if iteration is not valid in p.
 */
extern dword d_set_nextkey(d_set_t *);

#endif

/* EOF set.h */
