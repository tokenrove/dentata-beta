#!/bin/sh
# 
# test-set.sh
# Created: Sat Feb 24 23:41:49 2001 by tek@wiw.org
# Revised: Sun Jun 17 02:23:42 2001 by tek@wiw.org
# Copyright 2001 Julian E. C. Squires (tek@wiw.org)
# This program comes with ABSOLUTELY NO WARRANTY.
# $Id$
# 
# Perform a series of tests on dentata's set module using the
# test-set program.
#
# FIXME: We currently don't test the effects of working with multiple
# sets in parallel, as test-set does not support it.
#

test=./tests/test-set

# Test 1 [pass]
#
# Create a set, add 10000 elements, check set integrity,
# remove 10000 elements, delete the set.
$test -q -c n a10000 c r10000 d
if [ $? != 0 ]; then
    echo "test-set.sh: Test 1 failed!"
    exit 1
fi

# Test 2 [fail]
#
# Create a set, add 100 elements, remove 101 elements.
$test -q -c n a100 r101
if [ $? == 0 ]; then
    echo "test-set.sh: Test 2 failed!"
    exit 1
fi

# Test 3 [fail]
#
# Add an element.
$test -q -c a
if [ $? == 0 ]; then
    echo "test-set.sh: Test 3 failed!"
    exit 1
fi

# Test 4 [fail]
#
# Create a set, delete the set, add an element.
$test -q -c n d a
if [ $? == 0 ]; then
    echo "test-set.sh: Test 4 failed!"
    exit 1
fi

# Test 5 [pass]
#
# Create a set with sizehint 16, add 256 elements, remove 32 elements,
# add 256 elements, check, remove 128 elements, check, delete the set.
$test -q -c n16 a256 r32 a256 c r128 c d
if [ $? != 0 ]; then
    echo "test-set.sh: Test 5 failed!"
    exit 1
fi

# EOF test-set.sh
